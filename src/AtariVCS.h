#ifndef ATARI_VCS_EMU_ATARIVCS_H
#define ATARI_VCS_EMU_ATARIVCS_H

#include "Common.h"

class Cartridge;

class AtariVCS
{
public:
  AtariVCS();
  ~AtariVCS();

  void Init();
  bool LoadROM(const char* szFilePath);
  void RunToVBlank(CRGBA* pFrameBuffer, int16_t* pSampleBuffer, int* pSampleCount);

private:
  Cartridge* m_pCartridge;
};

#endif //ATARI_VCS_EMU_ATARIVCS_H
