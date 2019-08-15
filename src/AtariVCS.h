#ifndef ATARI_VCS_EMU_ATARIVCS_H
#define ATARI_VCS_EMU_ATARIVCS_H

#include "Common.h"

class Cartridge;
class MOS6502Core;
class Memory;
class TIACore;

class AtariVCS
{
public:
  AtariVCS();
  ~AtariVCS();

  void Init();
  bool LoadROM(const char* szFilePath);
  void RunToVBlank(CRGBA* pFrameBuffer, int16_t* pSampleBuffer, int* pSampleCount);
  void Reset();

private:
  Cartridge* m_pCartridge;
  MOS6502Core* m_pProcessor;
  Memory* m_pMemory;
  TIACore* m_pTIA;
};

#endif //ATARI_VCS_EMU_ATARIVCS_H
