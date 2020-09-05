#pragma once

#include "TIABase.hpp"

class TIABall : public TIABase
{
public:
	void SetVdelay(uint8_t value) { m_Vdelay = value; }
	void SetSize(uint8_t value) { m_Size = 1 << (value >> 4); }
	void SetHMove(int8_t value) { m_HMove = value >> 4; }
	void ApplyHMove() { m_Position -= m_HMove; }
	void ResetPos(uint8_t value) { m_Position = 0; };
	void UpdatePixel(uint16_t currentPos, uint8_t *pixel)
	{
		if (m_Position == 0)
			m_Position = currentPos + 12;
		if (currentPos < m_Position || currentPos >= (m_Position + m_Size))
			return;
		if (m_Vdelay)
			return;
		if (m_Enabled)
			*pixel = m_Colour;
	}

private:
	bool m_Enabled = false;
	bool m_Vdelay = false;
	int8_t m_HMove;
	uint8_t m_Size;
	uint8_t m_Colour;
	uint16_t m_Position;
};