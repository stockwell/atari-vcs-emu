#include "Common.h"

#include "AtariVCS.h"
#include "Cart.h"
#include "Memory.h"
#include "MOS6502Core.h"

AtariVCS::AtariVCS() {

}

AtariVCS::~AtariVCS() {

}

void AtariVCS::Init() {
  m_pCartridge = new Cartridge();
  m_pMemory = new Memory();
  m_pProcessor = new MOS6502Core();

  m_pCartridge->Init();
  m_pMemory->Init();
  m_pProcessor->Init(m_pMemory);

  m_pMemory->SetProcessor(m_pProcessor);
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
  m_pProcessor->Tick();
}
