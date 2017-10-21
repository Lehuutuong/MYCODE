#pragma once
extern const unsigned __int32 crc_32_tab[];

#define UPDC32(octet, crc)\
  (unsigned __int32)((crc_32_tab[(((unsigned __int32)(crc)) ^ ((unsigned char)(octet))) & 0xff] ^ (((unsigned __int32)(crc)) >> 8)))

unsigned __int32 crc32 (const unsigned char *data, int length);
unsigned __int32 crc32int (unsigned __int32 *data);
BOOL crc32_selftests (void);
