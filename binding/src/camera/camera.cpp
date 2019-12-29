#include "../binding.h"
#include<opencv2\opencv.hpp>  
#include<opencv2\highgui\highgui.hpp>  
#include<opencv2\imgproc\imgproc.hpp>  
#include<iostream>  
#include<stdio.h>

using namespace cv;
using namespace std;
extern struct pa_plugin gp;
class Camera {
public:
    Camera(int idx):cap(idx) {
    }
    int read(const char* path) {
        if (!cap.isOpened())
        {
            return -1;
        }
        cap.read(frame);
        return !imwrite(path, frame);
    }
    double get(int prop) {
        return cap.get(prop);
    }
    bool set(int prop, double val) {
        return cap.set(prop, val);
    }
private:
    VideoCapture cap;
    Mat frame;
};
std::map<int, Camera> gcap;
int cameraOpen(pa_context *ctx){
    if (gp.is_number(ctx, 0)) {
        int idx=gp.get_int(ctx, 0);
        gcap.insert(std::make_pair(idx,idx));
        gp.push_int(ctx, idx);
    }
    else {
        gp.push_int(ctx, -1);
    }
    return 1;
}
int cameraClose(pa_context *ctx) {
    if (gp.is_number(ctx, 0)) {
        int idx = gp.get_int(ctx, 0);
        gcap.erase(idx);
        gp.push_int(ctx, 0);
    }
    else {
        gp.push_int(ctx, -1);
    }
    return 1;
}
int cameraRead(pa_context *ctx) {
    if (gp.is_number(ctx, 0) && gp.is_string(ctx,1)) {
        int idx = gp.get_int(ctx, 0);
        const char* path = gp.get_string(ctx,1);
        auto iter=gcap.find(idx);
        
        if (iter != gcap.end()) {
            gp.push_int(ctx,iter->second.read(path));
        }
    }
    gp.push_int(ctx, -1);
    return 1;
}
int cameraGet(pa_context *ctx) {
    if (gp.is_number(ctx, 0) && gp.is_number(ctx, 1)) {
        int idx = gp.get_int(ctx, 0);
        int prop = gp.get_int(ctx, 1);
        auto iter = gcap.find(idx);
        if (iter != gcap.end()) {
            gp.push_number(ctx,iter->second.get(prop));
            return 1;
        }
    }
    gp.push_number(ctx, -1);
    return 1;
}
int cameraSet(pa_context *ctx) {
    if (gp.is_number(ctx, 0) && gp.is_number(ctx, 1) && gp.is_number(ctx, 2)) {
        int idx = gp.get_int(ctx, 0);
        int prop = gp.get_int(ctx, 1);
        double val = gp.get_number(ctx, 2);
        auto iter = gcap.find(idx);
        if (iter != gcap.end()) {
            gp.push_number(ctx, iter->second.set(prop,val));
            return 1;
        }
    }
    gp.push_number(ctx, -1);
    return 1;
}