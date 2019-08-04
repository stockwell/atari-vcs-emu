#include "Common.h"

#include "AtariVCS.h"
#include "Cart.h"

AtariVCS::AtariVCS() {

}

AtariVCS::~AtariVCS() {

}

void AtariVCS::Init() {
  m_pCartridge = new Cartridge();

  m_pCartridge->Init();
}

bool AtariVCS::LoadROM(const char *szFilePath) {
  return m_pCartridge->LoadFromFile(szFilePath);
}

void AtariVCS::RunToVBlank(CRGBA *pFrameBuffer, int16_t *pSampleBuffer, int *pSampleCount) {

}
