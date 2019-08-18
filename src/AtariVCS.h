#ifndef ATARI_VCS_EMU_ATARIVCS_H
#define ATARI_VCS_EMU_ATARIVCS_H

#include "Common.h"

#include <vector>

class Cartridge;
class MOS6502Core;
class Memory;
class TIACore;
class RIOTCore;

class AtariVCS
{
public:
  AtariVCS();
  ~AtariVCS();

  bool LoadROM(const char* szFilePath);
  bool LoadROM(std::vector<uint8_t>* romBuffer);
  void RunToVBlank(uint8_t* pFrameBuffer, int16_t* pSampleBuffer, int* pSampleCount);
  void Reset();

private:
  Cartridge* m_pCartridge;
  MOS6502Core* m_pProcessor;
  Memory* m_pMemory;
  TIACore* m_pTIA;
  RIOTCore* m_pRIOT;
};

#endif //ATARI_VCS_EMU_ATARIVCS_H
