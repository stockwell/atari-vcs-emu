#include "RIOTCore.h"

RIOTCore::RIOTCore() {
  m_SWCHA = 0xFF;
  m_SWACNT = 0x00;
  m_SWCHB = 0x0B;
  m_SWBCNT = 0x00;
  m_INTIM = 0x00;
}

void RIOTCore::Tick() {
  if (!--m_TimerPrescaleCnt) {
    if (!--m_INTIM) {
      /* After timer underflow the timer will switch to using no prescaler, but will enable the prescaler again after
       * a read to INTIM */
      m_INTIM |= 0xC0;
    }

    m_INTIM & 0xC0 ? m_TimerPrescaleCnt = 1 : m_TimerPrescaleCnt = m_TimerPrescale;
  }
}

void RIOTCore::Write(uint16_t address, uint8_t value) {
  switch(address) {
    case 294: {
      m_INTIM = value;
      m_TimerPrescale = m_TimerPrescaleCnt = 1;

      m_INTIM &= ~0xC0;
    } break;

    case 295: {
      m_INTIM = value;
      m_TimerPrescale = m_TimerPrescaleCnt = 8;

      m_INTIM &= ~0xC0;
    } break;

    case 296: {
      m_INTIM = value;
      m_TimerPrescale = m_TimerPrescaleCnt = 16;
      m_INTIM &= ~0xC0;
    } break;

    case 297: {
      m_INTIM = value;
      m_TimerPrescale = m_TimerPrescaleCnt = 64;

      m_INTIM &= ~0xC0;
    } break;

    default:
      break;
  }
}

uint8_t RIOTCore::Read(uint16_t address) {
  if (address == 284) {
    m_INTIM &= ~0x40;
    return m_INTIM;
  }

  if (address == 285) {
    return m_TIMINT;
  }

  return 0x00;
}

void RIOTCore::SetSWCHA(uint8_t val) {
  m_SWCHA |= val;
}

void RIOTCore::ClearSWCHA(uint8_t val) {
  m_SWCHA &= ~val;
}

void RIOTCore::SetSWCHB(uint8_t val) {
  m_SWCHB |= val;
}

void RIOTCore::ClearSWCHB(uint8_t val) {
  m_SWCHB &= ~val;
}