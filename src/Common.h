#ifndef ATARI_VCS_EMU_COMMON_H
#define ATARI_VCS_EMU_COMMON_H

#include <cstdint>
#include <cstdio>

#include "HexDump.h"

#define SafeDelete(pointer) if(pointer != NULL) {delete pointer; pointer = NULL;}
#define SafeDeleteArray(pointer) if(pointer != NULL) {delete [] pointer; pointer = NULL;}

#ifdef DEBUG
#define Log(M, ...) printf(M "\n", ##__VA_ARGS__)
#define LogHexDump(...) hexDump(__VA_ARGS__)
#else
#define Log(msg, ...) (void)0
#define LogHexDump(...) (void)0
#endif

struct CRGBA {
  CRGBA() = default;
  CRGBA(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha) {
    this->red = red;
    this->green = green;
    this->blue = blue;
    this->alpha = alpha;
  }
  uint8_t red;
  uint8_t green;
  uint8_t blue;
  uint8_t alpha;
};

#endif //ATARI_VCS_EMU_COMMON_H
