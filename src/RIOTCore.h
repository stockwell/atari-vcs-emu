#ifndef ATARI_VCS_EMU_RIOTCORE_H
#define ATARI_VCS_EMU_RIOTCORE_H

#include "Common.h"

class RIOTCore {
public:
  RIOTCore();

  ~RIOTCore();

  void Write(uint16_t address, uint8_t value);

  uint8_t Read(uint16_t address);

  void Tick();

private:
  uint8_t *m_pMem;
  uint8_t m_TimerCnt;
  uint8_t m_TimerPrescale;
  uint8_t m_TimerPrescaleCnt;
};

static const char* kRIOTReadRegisterNames[0x09] = {
    "Swcha",    /* 280 - Port A data register (joysticks...) */
    "Swacnt",   /* 281 - Port A data direction register (DDR) */
    "Swchb",    /* 282 - Port B data (console switches) */
    "Swbcnt",   /* 283 - Port B DDR */
    "Intim",    /* 284 - Timer output */

    "Tim1t",    /* 294 - set 1 clock interval */
    "Tim8t",    /* 295 - set 8 clock interval */
    "Tim64t",   /* 296 - set 64 clock interval */
    "T1024t"    /* 297 - set 1024 clock interval */
};

#endif //ATARI_VCS_EMU_RIOTCORE_H
