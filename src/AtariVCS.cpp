#include "Common.h"

#include "AtariVCS.h"
#include "Cart.h"
#include "Memory.h"
#include "MOS6502Core.h"
#include "TIACore.h"

AtariVCS::AtariVCS() {

}

AtariVCS::~AtariVCS() {
  SafeDelete(m_pCartridge)
  SafeDelete(m_pMemory)
  SafeDelete(m_pProcessor)
  SafeDelete(m_pTIA)

}

void AtariVCS::Init() {
  m_pCartridge = new Cartridge();
  m_pMemory = new Memory();
  m_pProcessor = new MOS6502Core();
  m_pTIA = new TIACore();

  m_pCartridge->Init();
  m_pMemory->Init();
  m_pProcessor->Init(m_pMemory);
  m_pTIA->Init();

  m_pMemory->SetProcessor(m_pProcessor);
  m_pMemory->SetTIA(m_pTIA);
  m_pMemory->Reset();
}

bool AtariVCS::LoadROM(const char *szFilePath) {
  if (!m_pCartridge->LoadFromFile(szFilePath)) {
    return false;
  }

  m_pMemory->LoadROM(m_pCartridge->GetROM());

  return true;
}

void AtariVCS::Reset() {
  m_pProcessor->Reset();
}

void AtariVCS::RunToVBlank(CRGBA *pFrameBuffer, int16_t *pSampleBuffer, int *pSampleCount) {
  int pixel_idx = 0;

  static int offset = 0;
  static bool direction = true;

  if (direction) {
    if (++offset == 0xFF) {
      direction = false;
    }
  } else {
    if (--offset == 0) {
      direction = true;
    }
  }

  /* 262 scanlines per frame */
  for (size_t scan_line = 0; scan_line < 262; scan_line++) {
    m_pProcessor->Resume(); // Resume 6507 which was halted due to write to WSYNC

    /* 228 clock counts per scanline */
    for (size_t h_clock = 0; h_clock < 228; h_clock++) {
      if ((h_clock % 3) == 0)
        m_pProcessor->Tick();

      // First 3 scanlines are vertical sync, followed by 37 scanlines of vertical blank.
      // Last 30 scanlines are overscan
      // First 68 clock cycles are the horizontal blanking period
      // 160 x 192 effective pixels
      if (scan_line >= 40 && scan_line < 232 && h_clock >= 68) {
        /* Test pattern */
        CRGBA colour;
        colour.red = (offset) & 0xFF;
        colour.green = (offset << 1) & 0xFF;
        colour.blue =  (offset << 2) & 0xFF;
        colour.alpha = 0xFF;

        pFrameBuffer[pixel_idx++] = colour; //tia.getPixel(pixel_idx++);
      }
    }
  }
}