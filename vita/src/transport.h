#pragma once
#include "capture.h"
int vs_wifi_init(void);
int vs_wifi_send_frame(const VsFrame*, unsigned int, unsigned long long);
void vs_wifi_shutdown(void);
