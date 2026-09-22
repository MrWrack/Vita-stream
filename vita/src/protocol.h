#pragma once
#include <stdint.h>
#define VSTR_MAGIC 0x52545356u
#define VSTR_VERSION 2
#pragma pack(push, 1)
typedef struct {
 uint32_t magic; uint16_t version; uint16_t header_size; uint32_t frame_id;
 uint64_t timestamp_us; uint16_t width,height,pitch,pixel_format;
 uint16_t chunk_index,chunk_count,payload_size;
} VstrPacketHeader;
#pragma pack(pop)
