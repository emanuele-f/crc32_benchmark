#include <stdlib.h>
#include <stdint.h>

static const uint32_t Polynomial = 0xEDB88320;

uint32_t crc32_bitwise(const void* data, size_t length) {
  uint32_t previousCrc32 = 0;
  uint32_t crc = ~previousCrc32;

  unsigned char* current = (unsigned char*) data;

  while (length--)   {
    crc ^= *current++;

    for (unsigned int j = 0; j < 8; j++)
      crc = (crc >> 1) ^ (-(int)(crc & 1) & Polynomial);
  }

  return ~crc; // same as crc ^ 0xFFFFFFFF
}
