#include <stdio.h>
#include <iostream>
#include "opencv2/opencv.hpp"
#include <vector>
#include <string>
#include "cvip.h"

using namespace Napi;
using namespace cv;
using namespace std;
struct CamMsg {
    vector<uchar> data;
    size_t len;
    int rows;
    int cols;
    int type;
};
class CameraWorker : public AsyncProgressWorker<CamMsg> {
public:
    CameraWorker(Function& callback, int idx)
        : AsyncProgressWorker(callback), _needRead(0), _fps(20), err(0), needClose(0) {
        gp.open(idx, CAP_DSHOW);
    }

    ~CameraWorker() {
        if (gp.isOpened())gp.release();
    }
    // This code will be executed on the worker thread
    void Execute(const ExecutionProgress& progress) {
        // Need to simulate cpu heavy task

        std::unique_lock<std::mutex> lock(_cvm);
        Mat frame;
        vector<unsigned char> img_encode;
        double sleepTime = 1000 / (_fps > 10 ? _fps : 10);
        while (!needClose)
        {
            if (_needRead > 0) {
                _needRead--;
            }
            else {
                _needRead = 0;
                _cv.wait(lock);
            }
            _lock.lock();
            if (gp.read(frame)) {
                //imshow(_name, frame);
                if (vw.isOpened()) {
                    vw << frame;
                }
                waitKey(sleepTime);

                cv::imencode(".jpg", frame, img_encode);
                _msg.data = img_encode;
                _msg.len = frame.step[0] * frame.rows;;
                _msg.rows = frame.rows;
                _msg.cols = frame.cols;
                _msg.type = frame.type();
                progress.Send(&_msg, 1);
            }
            _lock.unlock();
        }
    }
    bool isOpened() {
        return gp.isOpened();
    }
    void OnOK() {
        HandleScope scope(Env());
    }

    void OnProgress(const CamMsg* msg, size_t  count) {
        if (msg && count) {
            if (count > 1) { printf("has more!!!!"); }
            HandleScope scope(Env());
            auto obj = Object::New(Env());
            obj.Set("data", Buffer<uchar>::New(Env(), const_cast<uchar*>(msg->data.data()), msg->data.size()));
            obj.Set("height", Number::New(Env(), msg->rows));
            obj.Set("width", Number::New(Env(), msg->cols));
            obj.Set("type", Number::New(Env(), msg->type));
            Callback().Call({ obj });
        }
    }
    void close() {
        _lock.lock();
        needClose = true;
        gp.release();
        _cv.notify_one();
        _lock.unlock();
    }
    void read() {
        _needRead++;
        _cv.notify_one();
    }
    void startRecord(string path) {
        if (vw.isOpened())return;
        _lock.lock();
        int height = gp.get(cv::CAP_PROP_FRAME_HEIGHT);
        int width = gp.get(cv::CAP_PROP_FRAME_WIDTH);
        vw.open(path, VideoWriter::fourcc('D', 'I', 'V', 'X'), _fps, Size(width, height));
        _saveAction = 1;
        _lock.unlock();
    }
    void stopRecord() {
        _lock.lock();
        _saveAction = 0;
        if (vw.isOpened()) {
            vw.release();
        }
        _lock.unlock();
    }
    Value take(string path) {
        Mat frame;
        _lock.lock();
        bool ret = gp.read(frame);
        _lock.unlock();
        if (ret) {
            /*auto obj = Object::New(Env());
            obj.Set("data", Buffer<uchar>::Copy(Env(), const_cast<uchar*>(frame.data), frame.step[0] * frame.rows));
            obj.Set("height", Number::New(Env(), frame.rows));
            obj.Set("width", Number::New(Env(), frame.cols));
            obj.Set("type", Number::New(Env(), frame.type()));
            return obj;*/
            return Boolean::New(Env(), imwrite(path, frame));
        }
        return Env().Undefined();
    }
private:
    VideoCapture gp;
    VideoWriter vw;
    bool needClose;
    int err;
    std::condition_variable _cv;
    std::mutex _cvm;
    std::mutex _lock;
    string _name;
    int _saveAction;
    int _fps;
    int _needRead;
    CamMsg _msg;
};
std::set<CameraWorker*> glist;

int cvipGetCameraCount() {
    VideoCapture camera;
    int deviceCount = 0;
    while (true)
    {
        if (!camera.open(deviceCount, CAP_DSHOW)) {
            break;
        }
        deviceCount++;
        camera.release();
    }
    return deviceCount;
}

Value _cvipCameraOpen(const Napi::CallbackInfo& info) {
    auto env = info.Env();
    if (info[0].IsNumber() && info[1].IsFunction()) {
        int idx = info[0].ToNumber();
        Function cb = info[1].As<Function>();
        CameraWorker *wk = new CameraWorker(cb, idx);
        if (wk->isOpened()) {
            wk->Queue();
            glist.insert(wk);
            return External<CameraWorker>::New(info.Env(), wk);
        }
        delete wk;
    }
    else {
        Napi::Error::New(env, "call cameraOpen by incorrect parameter").ThrowAsJavaScriptException();
    }
    return env.Undefined();
}
Value _cvipCameraClose(const Napi::CallbackInfo& info) {
    auto env = info.Env();
    bool ret = false;
    if (info[0].IsExternal()) {
        CameraWorker* wk = info[0].As<External<CameraWorker>>().Data();
        if (glist.find(wk) != glist.end())
        {
            wk->close();
            glist.erase(wk);
        }
    }
    return env.Undefined();
}
Value _cvipCamereIsOpened(const Napi::CallbackInfo& info) {
    auto env = info.Env();
    bool ret = false;
    if (info[0].IsExternal()) {
        auto wk = info[0].As<External<CameraWorker>>().Data();
        if (glist.find(wk) != glist.end())ret = wk->isOpened();
    }
    return Napi::Boolean::New(env, ret);
}

Value _cvipCamereRead(const Napi::CallbackInfo& info) {
    auto env = info.Env();
    bool ret = false;
    if (info[0].IsExternal()) {
        CameraWorker* wk = info[0].As<External<CameraWorker>>().Data();
        if (glist.find(wk) != glist.end())
        {
            wk->read();
        }
    }
    return env.Undefined();
}
Value _cvipCamereTake(const Napi::CallbackInfo& info) {
    auto env = info.Env();
    bool ret = false;
    if (info[0].IsExternal() && info[1].IsString()) {
        CameraWorker* wk = info[0].As<External<CameraWorker>>().Data();
        if (glist.find(wk) != glist.end())
        {
            return wk->take(info[1].ToString());
        }
    }
    return env.Undefined();
}
Value _cvipCamereRecord(const Napi::CallbackInfo& info) {
    auto env = info.Env();
    bool ret = false;
    if (info[0].IsExternal()) {
        CameraWorker* wk = info[0].As<External<CameraWorker>>().Data();
        if (glist.find(wk) != glist.end())
        {
            if (info[1].IsString()) {
                wk->startRecord(info[1].ToString());
            }
            else {
                wk->stopRecord();
            }
        }
    }
    return env.Undefined();
}

Value _cvipCameraCount(const Napi::CallbackInfo& info) {
    auto env = info.Env();
    return Napi::Number::New(env, cvipGetCameraCount());
}