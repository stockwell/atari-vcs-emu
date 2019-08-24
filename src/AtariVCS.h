#ifndef ATARI_VCS_EMU_ATARIVCS_H
#define ATARI_VCS_EMU_ATARIVCS_H

#include "Common.h"

#include <vector>

class Cartridge;
class MOS6502Core;
class Memory;
class TIACore;
class RIOTCore;

typedef enum {
  KEYPRESS_UP,
  KEYPRESS_DOWN,
  KEYPRESS_LEFT,
  KEYPRESS_RIGHT,
  KEYPRESS_SPACE,
  KEYPRESS_RESET,
  KEYPRESS_SELECT
} keypress_event_t;

class AtariVCS {
public:
  AtariVCS();
  ~AtariVCS();

  bool LoadROM(const char* szFilePath);
  bool LoadROM(std::vector<uint8_t>* romBuffer);
  void RunToVBlank(uint8_t* pFrameBuffer, int16_t* pSampleBuffer, int* pSampleCount);
  void Reset();
  void KeypressEvent(keypress_event_t evt, bool pressed);

private:
  Cartridge* m_pCartridge;
  MOS6502Core* m_pProcessor;
  Memory* m_pMemory;
  TIACore* m_pTIA;
  RIOTCore* m_pRIOT;
};

#endif //ATARI_VCS_EMU_ATARIVCS_H
