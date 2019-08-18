#include "RIOTCore.h"

RIOTCore::RIOTCore() {
  m_pMem = new uint8_t[0x0F];
}

RIOTCore::~RIOTCore() {
  SafeDeleteArray(m_pMem)
}

void RIOTCore::Tick() {
  if (!--m_TimerPrescaleCnt) {
    m_TimerPrescaleCnt = m_TimerPrescale;

    if (!--m_TimerCnt) {
      /* After timer underflow the timer will switch to using no prescaler, but will enable the prescaler again after
       * a read to INTIM */
    }
  }
}

void RIOTCore::Write(uint16_t address, uint8_t value) {
  switch(address) {
    case 294: {
      m_TimerCnt = value;
      m_TimerPrescale = 1;
      m_TimerPrescaleCnt = 1;
    } break;

    case 295: {
      m_TimerCnt = value;
      m_TimerPrescale = 8;
    } break;

    case 296: {
      m_TimerCnt = value;
      m_TimerPrescale = 16;
    } break;

    case 297: {
      m_TimerCnt = value;
      m_TimerPrescale = 64;
    } break;

    default:
      break;
  }
}

uint8_t RIOTCore::Read(uint16_t address) {
  if (address == 284) {
    return m_TimerCnt;
  }

  return 0x00;
}

