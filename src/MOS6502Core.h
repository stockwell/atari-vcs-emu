#ifndef ATARI_VCS_EMU_MOS6502CORE_H
#define ATARI_VCS_EMU_MOS6502CORE_H

#include "Common.h"

class MOS6502Core
{
public:
  MOS6502Core();
  ~MOS6502Core();
  void Init();
  void Reset();
  uint8_t Tick();

private:
  uint16_t PC;
  uint8_t AC;
  uint8_t XR;
  uint8_t YR;
  uint8_t SR;
  uint8_t SP;

private:
  uint8_t FetchOPCode();
  void ExecuteOPCode(uint8_t opcode);
};



#endif //ATARI_VCS_EMU_MOS6502CORE_H
