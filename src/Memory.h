#ifndef ATARI_VCS_EMU_MEMORY_H
#define ATARI_VCS_EMU_MEMORY_H

#include "Common.h"

class MOS6502Core;

class Memory {
public:
  Memory();
  ~Memory();
  void Init();
  void Reset();
  void SetProcessor(MOS6502Core* pProcessor);
  uint8_t Read(uint16_t address);
  void Write(uint16_t address, uint8_t value);
  void Load(uint16_t address, uint8_t value);
  void LoadBuffer(uint16_t address, uint8_t *bytes, size_t numBytes);
  void LoadROM(uint8_t* pROM);

private:
  MOS6502Core* m_pProcessor;
  uint8_t* m_pMap;
};

#endif //ATARI_VCS_EMU_MEMORY_H
