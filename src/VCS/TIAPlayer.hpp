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

	void SetSize(uint8_t value)
	{
		m_Size = value & 0x07u;
		CalcBuffer();
	}

	void CalcBuffer()
	{
		memset(m_SpriteBuffer, 0x00, sizeof m_SpriteBuffer);

		switch (m_Size)
		{
			// One copy
			case 0:
				memcpy(&m_SpriteBuffer[0], &m_Sprite, sizeof m_Sprite);
				break;

			// Two copies - close
			case 1:
				memcpy(&m_SpriteBuffer[0], &m_Sprite, sizeof m_Sprite);
				memcpy(&m_SpriteBuffer[2], &m_Sprite, sizeof m_Sprite);
				break;

			// Two copies - med
			case 2:
				memcpy(&m_SpriteBuffer[0], &m_Sprite, sizeof m_Sprite);
				memcpy(&m_SpriteBuffer[4], &m_Sprite, sizeof m_Sprite);
				break;

			// Three copies - close
			case 3:
				memcpy(&m_SpriteBuffer[0], &m_Sprite, sizeof m_Sprite);
				memcpy(&m_SpriteBuffer[2], &m_Sprite, sizeof m_Sprite);
				memcpy(&m_SpriteBuffer[4], &m_Sprite, sizeof m_Sprite);
				break;

			// two copies - wide
			case 4:
				memcpy(&m_SpriteBuffer[0], &m_Sprite, sizeof m_Sprite);
				memcpy(&m_SpriteBuffer[8], &m_Sprite, sizeof m_Sprite);
				break;

			// double size player
			case 5:
				for (int i = 0, j = 0; i < 16; i++)
				{
					if (m_Sprite & 1u << j)
						m_SpriteBuffer[i] |= 1u << j;

					if (i % 2 == 1)
						++j;
				}
				break;

			// 3 copies med
			case 6:
				memcpy(&m_SpriteBuffer[0], &m_Sprite, sizeof m_Sprite);
				memcpy(&m_SpriteBuffer[4], &m_Sprite, sizeof m_Sprite);
				memcpy(&m_SpriteBuffer[8], &m_Sprite, sizeof m_Sprite);
				break;

			// quad sized player
			case 7:
				for (int i = 0, j = 0; i < 32; i++)
				{
					if (m_Sprite & (1 << j))
						m_SpriteBuffer[i] |= 1 << j;

					if (i % 4 == 3)
						++j;
				}
				break;
		}
	}

	void SetGraphics(uint8_t value)
	{
		if (m_Reflected)
			m_Sprite = value;
		else
			m_Sprite = bitreverse(value);

		CalcBuffer();
	}

	void SetReflected(bool value)
	{
		m_Reflected = value;

		if (m_Reflected)
			m_Sprite = bitreverse(m_Sprite);

		CalcBuffer();
	}

	void UpdatePixel(uint16_t currentPos, uint8_t *pixel)
	{
		if (m_Position == 0)
			m_Position = currentPos + 0;

		if (currentPos < m_Position || currentPos > (m_Position + 70) || m_Vdelay)
			return;

		uint8_t pixel_index = (currentPos - m_Position) % 8;
		uint8_t selected_buffer = (currentPos - m_Position) / 8;

		if (m_SpriteBuffer[selected_buffer] & (1u << pixel_index))
			*pixel = m_Colour;

	}

private:
	bool m_Reflected = false;
	bool m_Vdelay;
	int8_t m_HMove;
	uint8_t m_Colour;
	uint8_t m_Sprite;
	uint8_t m_Position;
	uint8_t m_Size = 0;
	uint8_t m_SpriteBuffer[10];
};
