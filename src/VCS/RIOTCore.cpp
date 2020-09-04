#include "RIOTCore.h"

RIOTCore::RIOTCore()
{
	m_SWCHA = 0xFF;
	m_SWACNT = 0x00;
	m_SWCHB = 0x0B;
	m_SWBCNT = 0x00;

	m_INTIM = 0x80;
	m_TimerPrescale = 1024;
	m_TimerPrescaleCnt = 0x400;
	m_Delay = 1;
}

void RIOTCore::Tick()
{
	if (--m_Delay)
		return;

	m_Delay = 3;

	if (!--m_TimerPrescaleCnt)
	{
		if (--m_INTIM < 0x00)
		{
			/* After timer underflow the timer will switch to using no prescaler, but will enable the prescaler again after
			 * a read to INTIM */
			m_INTIM |= 0xC0u;
		}

		m_INTIM & 0xC0u ? m_TimerPrescaleCnt = 1 : m_TimerPrescaleCnt = m_TimerPrescale;
	}
}

void RIOTCore::Write(uint16_t address, uint8_t value)
{
	switch (address)
	{
		case 0x294:
		{
			m_INTIM = value;
			m_TimerPrescale = m_TimerPrescaleCnt = 1;

			m_INTIM &= ~0xC0u;
			break;
		}

		case 0x295:
		{
			m_INTIM = value;
			m_TimerPrescale = m_TimerPrescaleCnt = 8;

			m_INTIM &= ~0xC0u;
			break;
		}

		case 0x296:
		{
			m_INTIM = value;
			m_TimerPrescale = m_TimerPrescaleCnt = 64;
			m_INTIM &= ~0xC0u;
			break;
		}


		case 0x297:
		{
			m_INTIM = value;
			m_TimerPrescale = m_TimerPrescaleCnt = 1024;

			m_INTIM &= ~0xC0u;
			break;
		}

		default:
			break;
	}
}

uint8_t RIOTCore::Read(uint16_t address)
{

	switch (address) {
		case 0x280:
			return m_SWCHA;

		case 0x282:
			return m_SWCHB;

		case 0x284:
			m_INTIM &= ~0x40u;
			return m_INTIM;

		case 0x285:
			return m_TIMINT;

		default:break;

	}

	return 0xFF;
}

void RIOTCore::SetSWCHA(uint8_t val)
{
	m_SWCHA |= val;
}

void RIOTCore::ClearSWCHA(uint8_t val)
{
	m_SWCHA &= ~val;
}

void RIOTCore::SetSWCHB(uint8_t val)
{
	m_SWCHB |= val;
}

void RIOTCore::ClearSWCHB(uint8_t val)
{
	m_SWCHB &= ~val;
}
