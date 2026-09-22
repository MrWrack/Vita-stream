#include <psp2/kernel/processmgr.h>
#include <psp2/kernel/threadmgr.h>
#include <psp2/net/net.h>
#include <stdlib.h>
#include "capture.h"
#include "transport.h"
static unsigned long long now_us(void){return (unsigned long long)sceKernelGetProcessTimeWide();}
int main(void){
 const unsigned netmem_size=1024*1024; void*netmem=malloc(netmem_size); SceNetInitParam p={netmem,netmem_size,0};
 if(sceNetInit(&p)<0)sceKernelExitProcess(1); if(vs_wifi_init()<0)sceKernelExitProcess(2);
 unsigned id=0; for(;;){VsFrame f;if(vs_capture_current(&f)==0)vs_wifi_send_frame(&f,id++,now_us());sceKernelDelayThread(33333);}
}
