#ifndef ATARI_VCS_EMU_RIOTCORE_H
#define ATARI_VCS_EMU_RIOTCORE_H

#include "Common.h"

class RIOTCore {
public:
  RIOTCore();

  ~RIOTCore() = default;

  void Write(uint16_t address, uint8_t value);

  uint8_t Read(uint16_t address);

  void Tick();

  void SetSWCHA(uint8_t val);

  void ClearSWCHA(uint8_t val);

  void SetSWCHB(uint8_t val);

  void ClearSWCHB(uint8_t val);

private:
  uint16_t m_TimerPrescale;
  uint16_t m_TimerPrescaleCnt;
  uint8_t m_SWCHA;
  uint8_t m_SWCHB;
  uint8_t m_SWACNT;
  uint8_t m_SWBCNT;
  uint8_t m_INTIM;
  uint8_t m_TIMINT;
};

static const char* kRIOTReadRegisterNames[0x0A] = {
    "Swcha",    /* 280 - Port A data register (joysticks...) */
    "Swacnt",   /* 281 - Port A data direction register (DDR) */
    "Swchb",    /* 282 - Port B data (console switches) */
    "Swbcnt",   /* 283 - Port B DDR */
    "Intim",    /* 284 - Timer output */
    "Timint",   /* 285 - Timer Interrupt */

    "Tim1t",    /* 294 - set 1 clock interval */
    "Tim8t",    /* 295 - set 8 clock interval */
    "Tim64t",   /* 296 - set 64 clock interval */
    "T1024t"    /* 297 - set 1024 clock interval */
};

#endif //ATARI_VCS_EMU_RIOTCORE_H
