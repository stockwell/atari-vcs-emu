#include "Controller.hpp"

Controller::Controller()
{
	m_keyStateCached.resize(TotalButtons);

	m_keyMap[KEYPRESS_UP] = Up;
	m_keyMap[KEYPRESS_DOWN] = Down;
	m_keyMap[KEYPRESS_LEFT] = Left;
	m_keyMap[KEYPRESS_RIGHT] = Right;

	m_keyMap[KEYPRESS_A] = A;
	m_keyMap[KEYPRESS_B] = B;

	m_keyMap[KEYPRESS_START] = Start;
	m_keyMap[KEYPRESS_SELECT] = Select;
}

void Controller::Strobe(uint8_t b)
{
	m_strobe = (b & 1);
	if (! m_strobe)
	{
		m_keyStates = 0;
		int shift = 0;
		for (int button = A; button < TotalButtons; ++button)
		{
			m_keyStates |= m_keyStateCached[button] << shift;
			++shift;
		}
	}
}

uint8_t Controller::Read()
{
	uint8_t ret;
	if (m_strobe)
		ret = m_keyStateCached[A];
	else
	{
		ret = (m_keyStates & 1);
		m_keyStates >>= 1;
	}
	return ret | 0x40;
}

void Controller::HandleKeypress(keypress_event_t evt, bool pressed)
{
	m_keyStateCached[m_keyMap[evt]] = pressed;
}
