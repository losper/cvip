#include "binding.h"
#include "cvip/cvip.h"
#include "camera/camera.h"
#include <iostream>

extern struct pa_plugin gp;
int jsCvipEncode(pa_context *ctx) {
	if (gp.is_string(ctx, 0) && gp.is_string(ctx, 1) && gp.is_number(ctx, 2)) {
		const char* src = gp.get_string(ctx, 0);
		const char* dst = gp.get_string(ctx, 1);
		int cvfmt = gp.get_int(ctx, 2);
		gp.push_int(ctx, cvipEncode(src, dst, cvfmt));
	}
	else {
		gp.push_int(ctx, -2);
	}
	return 1;
}

int jsCvipMatch(pa_context *ctx) {
	if (gp.is_string(ctx, 0) && gp.is_string(ctx, 1)) {
		const char* src = gp.get_string(ctx, 0);
		const char* dst = gp.get_string(ctx, 1);
		stPos stp = cvipMatch(src, dst);
		int obj_idx = gp.push_object(ctx);
		gp.push_int(ctx, stp.status);
		gp.put_prop_string(ctx, obj_idx, "valid");
		gp.push_int(ctx, stp.x);
		gp.put_prop_string(ctx, obj_idx, "x");
		gp.push_int(ctx, stp.y);
		gp.put_prop_string(ctx, obj_idx, "y");
		gp.push_number(ctx, stp.val);
		gp.put_prop_string(ctx, obj_idx, "val");
	}
	else {
		const char* src = gp.get_string(ctx, 0);
		const char* dst = gp.get_string(ctx, 1);
		stPos stp = cvipMatch(src, dst);
		int obj_idx = gp.push_object(ctx);
		gp.push_int(ctx, 0);
		gp.put_prop_string(ctx, obj_idx, "valid");
		gp.push_int(ctx, 0);
		gp.put_prop_string(ctx, obj_idx, "x");
		gp.push_int(ctx, 0);
		gp.put_prop_string(ctx, obj_idx, "y");
		gp.push_number(ctx, stp.val);
		gp.put_prop_string(ctx, obj_idx, "val");
	}
	return 1;
}

int jsCvipCut(pa_context *ctx) {
    if (gp.is_string(ctx, 0) && gp.is_string(ctx, 1) && gp.is_number(ctx, 2) && gp.is_number(ctx, 3) && gp.is_number(ctx, 4) && gp.is_number(ctx, 5) && gp.is_number(ctx, 6)) {
        const char* src = gp.get_string(ctx, 0);
        const char* dst = gp.get_string(ctx, 1);
        int cvfmt = gp.get_int(ctx, 2);
        gp.push_int(ctx, cvipCut(src, dst, cvfmt, gp.get_int(ctx, 3), gp.get_int(ctx, 4), gp.get_int(ctx, 5), gp.get_int(ctx, 6)));
    }
    else {
        gp.push_int(ctx, -2);
    }
    return 1;
}


