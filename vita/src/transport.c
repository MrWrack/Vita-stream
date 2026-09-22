#include "transport.h"
#include "config.h"
#include "protocol.h"
#include <psp2/net/net.h>
#include <string.h>
static int sock=-1; static SceNetSockaddrIn dst;
int vs_wifi_init(void){
 sock=sceNetSocket("VitaStream",SCE_NET_AF_INET,SCE_NET_SOCK_DGRAM,0); if(sock<0)return sock;
 memset(&dst,0,sizeof(dst)); dst.sin_family=SCE_NET_AF_INET; dst.sin_port=sceNetHtons(PC_PORT);
 sceNetInetPton(SCE_NET_AF_INET,PC_IP,&dst.sin_addr); return 0;
}
int vs_wifi_send_frame(const VsFrame*f,unsigned int id,unsigned long long ts){
 if(!f||!f->base||sock<0)return -1; const unsigned bpp=4,total=f->pitch*f->height*bpp;
 const unsigned char*src=(const unsigned char*)f->base; const unsigned chunks=(total+VSTR_MAX_PAYLOAD-1)/VSTR_MAX_PAYLOAD;
 unsigned char packet[sizeof(VstrPacketHeader)+VSTR_MAX_PAYLOAD];
 for(unsigned i=0;i<chunks;i++){ unsigned off=i*VSTR_MAX_PAYLOAD, rem=total-off, n=rem>VSTR_MAX_PAYLOAD?VSTR_MAX_PAYLOAD:rem;
  VstrPacketHeader h={VSTR_MAGIC,VSTR_VERSION,sizeof(VstrPacketHeader),id,ts,(uint16_t)f->width,(uint16_t)f->height,(uint16_t)f->pitch,(uint16_t)f->format,(uint16_t)i,(uint16_t)chunks,(uint16_t)n};
  memcpy(packet,&h,sizeof(h)); memcpy(packet+sizeof(h),src+off,n);
  sceNetSendto(sock,packet,sizeof(h)+n,0,(SceNetSockaddr*)&dst,sizeof(dst));
 } return 0;
}
void vs_wifi_shutdown(void){if(sock>=0)sceNetSocketClose(sock);sock=-1;}
