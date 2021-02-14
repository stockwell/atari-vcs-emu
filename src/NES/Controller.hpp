#pragma once

#include <cstdint>
#include <unordered_map>

#include "EmulatorCore.hpp"

class Controller
{
public:
	enum Buttons
	{
		Invalid,
		A,
		B,
		Select,
		Start,
		Up,
		Down,
		Left,
		Right,
		TotalButtons,
	};

	Controller();

	void 	Strobe(uint8_t b);
	uint8_t	Read();

	void 	HandleKeypress(keypress_event_t evt, bool pressed);
	
private:
	bool 		m_strobe = false;
	uint16_t	m_keyStates = 0;
	std::vector<bool> m_keyStateCached;
	std::unordered_map<keypress_event_t, Buttons> m_keyMap;
};
