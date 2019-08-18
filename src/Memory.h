#ifndef ATARI_VCS_EMU_MEMORY_H
#define ATARI_VCS_EMU_MEMORY_H

#include "Common.h"

class MOS6502Core;
class TIACore;
class RIOTCore;

class Memory {
public:
  Memory();
  ~Memory();
  void Reset();
  void SetProcessor(MOS6502Core* pProcessor);
  void SetTIA(TIACore *pTIA);
  void SetRIOT(RIOTCore *pRIOT);
  uint8_t Read(uint16_t address);
  void Write(uint16_t address, uint8_t value);
  void Load(uint16_t address, uint8_t value);
  void Load(uint16_t address, uint8_t *bytes, size_t numBytes);
  void LoadROM(const uint8_t* pROM, uint16_t offset);

private:
  MOS6502Core *m_pProcessor;
  TIACore *m_pTIA;
  RIOTCore *m_pRIOT;
  uint8_t *m_pMap;
};

#endif //ATARI_VCS_EMU_MEMORY_H
