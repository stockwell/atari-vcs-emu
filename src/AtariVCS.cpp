#include "Common.h"

#include "AtariVCS.h"
#include "Cart.h"
#include "Memory.h"
#include "MOS6502Core.h"
#include "TIACore.h"

AtariVCS::AtariVCS() {

}

AtariVCS::~AtariVCS() {

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

  /* 262 scanlines per frame */
  for (size_t scan_line = 0; scan_line < 262; scan_line++) {
    m_pProcessor->Resume(); // Resume 6507 which was halted due to write to WSYNC

    /* 228 clock counts per scanline */
    for (size_t v_clock = 0; v_clock < 228; v_clock++) {
      if ((v_clock % 3) == 0)
        m_pProcessor->Tick();

      // First 3 scanlines are vertical sync, followed by 37 scanlines of vertical blank.
      // Last 30 scanlines are overscan
      // First 68 clock cycles are the horizontal blanking period
      // 160 x 192 effective pixels
      if (scan_line >= 40 && scan_line <= 132 && v_clock >= 68) {
        //pFrameBuffer[pixel_idx] = tia.getPixel(pixel_idx++);
      }
    }
  }
}
