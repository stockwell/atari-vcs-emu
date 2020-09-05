#pragma once

#include <cstdint>

class TIABase
{
public:
	void SetColor(uint8_t colour) { m_Colour = colour; };
	uint8_t GetColour() { return m_Colour; };

	void SetEnable(bool enabled) { m_Enabled = enabled; }

	virtual void Tick() {};

protected:
	uint8_t m_Colour = 0;
	bool m_Enabled = false;
};
