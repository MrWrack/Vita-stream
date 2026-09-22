#include <psp2/kernel/processmgr.h>
#include <psp2/kernel/threadmgr.h>
#include <psp2/net/net.h>
#include <psp2/net/netctl.h>
#include <psp2/sysmodule.h>
#include <stdlib.h>
#include "capture.h"
#include "transport.h"

static void *g_netmem = NULL;
static int g_net_started = 0;
static int g_netctl_started = 0;

static unsigned long long now_us(void) {
    return (unsigned long long)sceKernelGetProcessTimeWide();
}

static int network_start(void) {
    int rc = sceSysmoduleLoadModule(SCE_SYSMODULE_NET);
    if (rc < 0 && rc != SCE_SYSMODULE_LOADED)
        return rc;

    const unsigned int netmem_size = 1024 * 1024;
    g_netmem = malloc(netmem_size);
    if (!g_netmem)
        return -1;

    SceNetInitParam p;
    p.memory = g_netmem;
    p.size = netmem_size;
    p.flags = 0;

    rc = sceNetInit(&p);
    if (rc < 0) {
        free(g_netmem);
        g_netmem = NULL;
        sceSysmoduleUnloadModule(SCE_SYSMODULE_NET);
        return rc;
    }
    g_net_started = 1;

    rc = sceNetCtlInit();
    if (rc < 0) {
        sceNetTerm();
        g_net_started = 0;
        free(g_netmem);
        g_netmem = NULL;
        sceSysmoduleUnloadModule(SCE_SYSMODULE_NET);
        return rc;
    }
    g_netctl_started = 1;
    return 0;
}

static void network_stop(void) {
    vs_wifi_shutdown();
    if (g_netctl_started) {
        sceNetCtlTerm();
        g_netctl_started = 0;
    }
    if (g_net_started) {
        sceNetTerm();
        g_net_started = 0;
    }
    if (g_netmem) {
        free(g_netmem);
        g_netmem = NULL;
    }
    sceSysmoduleUnloadModule(SCE_SYSMODULE_NET);
}

int main(void) {
    int rc = network_start();
    if (rc < 0) {
        /* Do not continue into socket/capture code after failed NET init. */
        sceKernelDelayThread(3 * 1000 * 1000);
        sceKernelExitProcess(rc);
        return rc;
    }

    rc = vs_wifi_init();
    if (rc < 0) {
        network_stop();
        sceKernelDelayThread(3 * 1000 * 1000);
        sceKernelExitProcess(rc);
        return rc;
    }

    unsigned int frame_id = 0;
    for (;;) {
        VsFrame frame;
        if (vs_capture_current(&frame) == 0)
            vs_wifi_send_frame(&frame, frame_id++, now_us());

        /* Start conservatively at ~15 FPS while Wi-Fi transport is validated. */
        sceKernelDelayThread(66666);
    }

    network_stop();
    return 0;
}
