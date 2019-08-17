#include "Common.h"

#include "AtariVCS.h"
#include "Cart.h"
#include "Memory.h"
#include "MOS6502Core.h"
#include "TIACore.h"

AtariVCS::~AtariVCS() {
  SafeDelete(m_pCartridge)
  SafeDelete(m_pMemory)
  SafeDelete(m_pProcessor)
  SafeDelete(m_pTIA)
}

void AtariVCS::Init() {
  m_pCartridge = new Cartridge();
  m_pMemory = new Memory();
  m_pProcessor = new MOS6502Core(m_pMemory);
  m_pTIA = new TIACore(m_pProcessor);

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

bool AtariVCS::LoadROM(std::vector<uint8_t>* romBuffer) {
  if (!m_pCartridge->LoadFromBuffer(romBuffer->data(), romBuffer->size())) {
    return false;
  }

  m_pMemory->LoadROM(m_pCartridge->GetROM());
  return true;
}

void AtariVCS::Reset() {
  m_pProcessor->Reset();
}

void AtariVCS::RunToVBlank(uint8_t *pFrameBuffer, int16_t *pSampleBuffer, int *pSampleCount) {
  m_pProcessor->Tick();
  m_pTIA->Tick(pFrameBuffer);
}
