#include "cv.h"
#include "cvip.h"
#include <WinSock.h>
#include <ws2bth.h>
struct pa_plugin gp;
static const pa_function_list_entry my_module_funcs[] = {
    { "stdev", stdev, PA_VARARGS /*nargs*/ },
    { "absdiff", absdiff, PA_VARARGS /*nargs*/ },
    { "horizontal", horizontal, PA_VARARGS /*nargs*/ },
    { "resave", resave, PA_VARARGS /*nargs*/ },
    { "cutcap", cutcap, PA_VARARGS /*nargs*/ },
    { "jsCvipMatch", jsCvipMatch, PA_VARARGS /*nargs*/ },
    { "jsCvipEncode", jsCvipEncode, PA_VARARGS /*nargs*/ },
    { "jsCvipCut", jsCvipCut, PA_VARARGS /*nargs*/ },
    { 0, 0, 0 }
};
extern "C" int passoa_init(pa_plugin p) {
    gp = p;
    gp.put_function_list(p.ctx, -1, my_module_funcs);
    return 0;
}
extern "C" int passoa_exit() {
    return 0;
}


