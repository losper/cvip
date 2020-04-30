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
    uchar* data;
    size_t len;
    int rows;
    int cols;
    int type;
};
class CameraWorker : public AsyncProgressWorker<CamMsg> {
public:
    CameraWorker(Function& callback, int idx)
        : AsyncProgressWorker(callback), err(0), needClose(0) {
        gp.open(idx, CAP_DSHOW);
    }

    ~CameraWorker() {
        if (gp.isOpened())gp.release();
    }
    // This code will be executed on the worker thread
    void Execute(const ExecutionProgress& progress) {
        // Need to simulate cpu heavy task
        CamMsg msg;
        std::unique_lock<std::mutex> lock(_cvm);
        Mat frame;
        while (!needClose)
        {
            _cv.wait(lock);
            if (gp.read(frame)) {
                msg.data = frame.data;
                msg.len = frame.elemSize();
                msg.rows = frame.rows;
                msg.cols = frame.cols;
                msg.type = frame.type();
                progress.Send(&msg, 1);
            }
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
            Callback().Call({ Buffer<uchar>::New(Env(),const_cast<uchar*>(msg->data),msg->len) });
        }
    }
    void close() {
        needClose = true; gp.release();
    }
    void read() {
        _cv.notify_one();
    }
private:
    VideoCapture gp;
    bool needClose;
    int err;
    std::condition_variable _cv;
    std::mutex _cvm;
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
        wk->Queue();
        glist.insert(wk);
        return External<CameraWorker>::New(info.Env(), wk);
    }
    else {
        Napi::Error::New(env, "call cameraOpen by incorrect parameter").ThrowAsJavaScriptException();
    }
    return;
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

Value _cvipCameraCount(const Napi::CallbackInfo& info) {
    auto env = info.Env();
    return Napi::Number::New(env, cvipGetCameraCount());
}