#include "plugin.h"

int jsCvipEncode(pa_context* ctx);
int jsCvipMatch(pa_context* ctx);
int jsCvipCut(pa_context *ctx);
int cameraOpen(pa_context *ctx);
int cameraClose(pa_context *ctx);
int cameraRead(pa_context *ctx);
int cameraGet(pa_context *ctx);
int cameraSet(pa_context *ctx);