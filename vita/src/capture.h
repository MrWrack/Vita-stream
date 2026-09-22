#pragma once
#include <psp2/display.h>
typedef struct { const void *base; unsigned int pitch,width,height; SceDisplayPixelFormat format; } VsFrame;
int vs_capture_current(VsFrame *out);
