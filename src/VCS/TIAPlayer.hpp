#pragma once

#include "Common.h"

#include "TIABase.hpp"

class TIAPlayer : public TIABase
{
public:
	void ResetPos(uint8_t value) { m_Position = 0; };
	uint16_t GetPos() { return m_Position; }
	void SetHMove(int8_t value)	{ m_HMove = value >> 4; }
	void ApplyHMove() { m_Position -= m_HMove; }
	void SetVdelay(uint8_t value) { m_Vdelay = value; }

	void SetSize(uint8_t value);
	void SetGraphics(uint8_t value);
	void SetReflected(bool value);

	void UpdatePixel(uint16_t currentPos, uint8_t *pixel);

private:
	void CalcBuffer();

private:
	bool m_Reflected = false;
	bool m_Vdelay = false;
	int8_t m_HMove = 0;
	uint8_t m_Sprite = 0;
	uint8_t m_Position = 0;
	uint8_t m_Size = 0;
	uint8_t m_SpriteBuffer[10] = {};
};
