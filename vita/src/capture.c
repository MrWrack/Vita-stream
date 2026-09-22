#include "capture.h"
#include <string.h>
int vs_capture_current(VsFrame *out) {
 if (!out) return -1;
 SceDisplayFrameBuf fb; memset(&fb,0,sizeof(fb)); fb.size=sizeof(fb);
 int rc=sceDisplayGetFrameBuf(&fb,SCE_DISPLAY_SETBUF_IMMEDIATE); if(rc<0)return rc;
 out->base=fb.base; out->pitch=fb.pitch; out->width=fb.width; out->height=fb.height; out->format=fb.pixelformat; return 0;
}
