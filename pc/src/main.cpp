#define WIN32_LEAN_AND_MEAN
#include <winsock2.h>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <unordered_map>
#include <vector>
#include <chrono>
#pragma comment(lib,"Ws2_32.lib")
#pragma pack(push,1)
struct Header{std::uint32_t magic;std::uint16_t version,header_size;std::uint32_t frame_id;std::uint64_t timestamp_us;std::uint16_t width,height,pitch,pixel_format,chunk_index,chunk_count,payload_size;};
#pragma pack(pop)
static constexpr std::uint32_t MAGIC=0x52545356u; static constexpr std::size_t PAYLOAD=1200;
struct Pending{Header first{};std::vector<std::uint8_t>pixels;std::vector<bool>got;unsigned received=0;std::chrono::steady_clock::time_point born;};
int main(int argc,char**argv){
 int port=argc>1?std::atoi(argv[1]):5000; WSADATA w{};if(WSAStartup(MAKEWORD(2,2),&w))return 1;
 SOCKET s=socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);sockaddr_in a{};a.sin_family=AF_INET;a.sin_addr.s_addr=htonl(INADDR_ANY);a.sin_port=htons((u_short)port);
 if(bind(s,(sockaddr*)&a,sizeof(a))==SOCKET_ERROR)return 2;
 std::cout<<"PS Vita Stream v0.2 - Made by MrWrack\nListening on UDP "<<port<<"...\n";
 std::unordered_map<std::uint32_t,Pending>frames;std::uint32_t newest=0;std::vector<std::uint8_t>buf(1500);
 for(;;){int n=recvfrom(s,(char*)buf.data(),(int)buf.size(),0,nullptr,nullptr);if(n<(int)sizeof(Header))continue;Header h{};memcpy(&h,buf.data(),sizeof(h));
  if(h.magic!=MAGIC||h.version!=2||h.header_size!=sizeof(Header)||h.payload_size>PAYLOAD||sizeof(Header)+h.payload_size>(size_t)n||!h.chunk_count||h.chunk_index>=h.chunk_count)continue;
  auto&f=frames[h.frame_id];if(f.got.empty()){f.first=h;f.got.assign(h.chunk_count,false);f.pixels.resize((size_t)h.chunk_count*PAYLOAD);f.born=std::chrono::steady_clock::now();}
  if(f.got.size()!=h.chunk_count){frames.erase(h.frame_id);continue;}if(!f.got[h.chunk_index]){memcpy(f.pixels.data()+(size_t)h.chunk_index*PAYLOAD,buf.data()+sizeof(Header),h.payload_size);f.got[h.chunk_index]=true;++f.received;}
  if(f.received==h.chunk_count){if(h.frame_id>=newest){newest=h.frame_id;std::cout<<"\rLatest frame "<<newest<<" "<<h.width<<"x"<<h.height<<"   "<<std::flush;}for(auto it=frames.begin();it!=frames.end();)if(it->first<=newest)it=frames.erase(it);else++it;}
  auto now=std::chrono::steady_clock::now();for(auto it=frames.begin();it!=frames.end();)if(std::chrono::duration_cast<std::chrono::milliseconds>(now-it->second.born).count()>50)it=frames.erase(it);else++it;
 }}
