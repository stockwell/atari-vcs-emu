#include "Common.h"

#include "AtariVCS.h"
#include "Cart.h"
#include "Memory.h"
#include "MOS6502Core.h"
#include "TIACore.h"
#include "RIOTCore.h"

AtariVCS::AtariVCS() {
  m_pCartridge = new Cartridge();
  m_pMemory = new Memory();
  m_pProcessor = new MOS6502Core(m_pMemory);
  m_pTIA = new TIACore(m_pProcessor);
  m_pRIOT = new RIOTCore();

  m_pMemory->SetProcessor(m_pProcessor);
  m_pMemory->SetTIA(m_pTIA);
  m_pMemory->SetRIOT(m_pRIOT);

  m_pMemory->Reset();
}

AtariVCS::~AtariVCS() {
  SafeDelete(m_pCartridge)
  SafeDelete(m_pMemory)
  SafeDelete(m_pProcessor)
  SafeDelete(m_pTIA)
  SafeDelete(m_pRIOT);
}

bool AtariVCS::LoadROM(const char *szFilePath) {
  if (!m_pCartridge->LoadFromFile(szFilePath)) {
    return false;
  }

  m_pMemory->LoadROM(m_pCartridge->GetROM(), m_pCartridge->GetRomSize());

  return true;
}

bool AtariVCS::LoadROM(std::vector<uint8_t>* romBuffer) {
  if (!m_pCartridge->LoadFromBuffer(romBuffer->data(), romBuffer->size())) {
    return false;
  }

  m_pMemory->LoadROM(m_pCartridge->GetROM(), m_pCartridge->GetRomSize());
  return true;
}

void AtariVCS::Reset() {
  m_pProcessor->Reset();
}

void AtariVCS::RunToVBlank(uint8_t *pFrameBuffer, int16_t *pSampleBuffer, int *pSampleCount) {
  while (1) {
    m_pProcessor->Tick();
    m_pRIOT->Tick();

    if (m_pTIA->Tick(pFrameBuffer))
      break;
  }
}

void AtariVCS::KeypressEvent(keypress_event_t evt, bool pressed) {
  switch (evt) {
    case KEYPRESS_UP:
      if (!pressed) {
        m_pRIOT->SetSWCHA(0x10);
      } else {
        m_pRIOT->ClearSWCHA(0x10);
      }
      break;

    case KEYPRESS_DOWN:
      if (!pressed) {
        m_pRIOT->SetSWCHA(0x20);
      } else {
        m_pRIOT->ClearSWCHA(0x20);
      }
      break;

    case KEYPRESS_LEFT:
      if (!pressed) {
        m_pRIOT->SetSWCHA(0x40);
      } else {
        m_pRIOT->ClearSWCHA(0x40);
      }
      break;

    case KEYPRESS_RIGHT:
      if (!pressed) {
        m_pRIOT->SetSWCHA(0x80);
      } else {
        m_pRIOT->ClearSWCHA(0x80);
      }
      break;

    case KEYPRESS_SPACE:
      m_pTIA->SetTrigger(0, pressed);
      break;

    case KEYPRESS_SELECT:
      if (!pressed) {
        m_pRIOT->SetSWCHB(0x02);
      } else {
        m_pRIOT->ClearSWCHB(0x02);
      }
      break;

    case KEYPRESS_RESET:
      if (!pressed) {
        m_pRIOT->SetSWCHB(0x01);
      } else {
        m_pRIOT->ClearSWCHB(0x01);
      }
      break;
  }
}
