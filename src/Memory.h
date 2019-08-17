#ifndef ATARI_VCS_EMU_MEMORY_H
#define ATARI_VCS_EMU_MEMORY_H

#include "Common.h"

class MOS6502Core;
class TIACore;

class Memory {
public:
  Memory();
  ~Memory();
  void Reset();
  void SetProcessor(MOS6502Core* pProcessor);
  void SetTIA(TIACore *pTIA);
  uint8_t Read(uint16_t address);
  void Write(uint16_t address, uint8_t value);
  void Load(uint16_t address, uint8_t value);
  void Load(uint16_t address, uint8_t *bytes, size_t numBytes);
  void LoadROM(const uint8_t* pROM);

private:
  MOS6502Core* m_pProcessor;
  TIACore* m_pTIA;
  uint8_t* m_pMap;
};

#endif //ATARI_VCS_EMU_MEMORY_H
