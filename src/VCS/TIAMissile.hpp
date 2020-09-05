#pragma once

#include "TIABase.hpp"

class TIAMissile : public TIABase
{
public:
	void SetSize(uint8_t value) { m_Size = 1 << (value >> 4); }
	void SetHMove(int8_t value) { m_HMove = value >> 4; }
	void ApplyHMove() { m_Position -= m_HMove; }
	void ResetPos(uint8_t value) { m_Position = 0; };
	void SetPos(uint16_t pos) { m_Position = pos; }
	void UpdatePixel(uint16_t currentPos, uint8_t *pixel)
	{
		if (m_Position == 0)
			m_Position = currentPos;
		if (currentPos < m_Position || currentPos >= (m_Position + m_Size))
			return;
		if (m_Enabled)
			*pixel = m_Colour;
	}

private:
	bool m_Enabled = false;
	int8_t m_HMove = 0;
	uint8_t m_Size = 1;
	uint8_t m_Colour;
	uint16_t m_Position;
};