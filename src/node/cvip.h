#ifndef __DPZ__CVIP__HPP__
#define __DPZ__CVIP__HPP__

#include <napi.h>
using namespace Napi;

struct stPos {
    int status;
    int x;
    int y;
    double val;
};
stPos cvipMatch(std::string src, std::string dst);
int cvipEncode(std::string src, std::string dst, int cvfmt);
int cvipCut(std::string src, std::string dst, int cvfmt, int x, int y, int w, int h);
Value _cvipCameraCount(const Napi::CallbackInfo& info);
Value _cvipCameraOpen(const Napi::CallbackInfo& info);
Value _cvipCameraClose(const Napi::CallbackInfo& info);
Value _cvipCamereIsOpened(const Napi::CallbackInfo& info);
Value _cvipCamereRead(const Napi::CallbackInfo& info);
Value _cvipCamereTake(const Napi::CallbackInfo& info);
Value _cvipCamereRecord(const Napi::CallbackInfo& info);
#endif
