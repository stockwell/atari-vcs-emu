#pragma once

#include <cstdint>

class TIABase
{
public:
	bool IsMoving()
	{
		return m_isMoving;
	}

	bool IsEnabled()
	{
		return m_Collision & 0x8000;
	}

	void SetHMx(uint8_t value)
	{
		m_HmxClocks = (value >> 4) ^ 0x08;
	}

	void SetHmove()
	{
		m_isMoving = true;
	}

	virtual void SetColor(uint8_t colour)
	{
		if (colour != m_Colour)
			m_Colour = colour;
	}

	virtual uint8_t GetColour()
	{
		return m_Colour;
	}

	virtual void SetEnable(bool enabled)
	{
		m_Enabled = enabled;
	}

	virtual void Tick(uint32_t x, uint32_t hcount) {};
	virtual void MovementTick(uint32_t clock, uint8_t hclock, bool hblank) {};
	virtual void NextLine() {};

public:
	uint32_t m_Collision = 0;

protected:
	static constexpr uint32_t kCollisionMaskEnabled = 0xFFFF;
	uint32_t m_CollisionMaskDisabled = 0;

	uint8_t m_Colour = 0;
	uint8_t m_Counter = 0;
	uint8_t m_HmxClocks = 0;
	int8_t m_RenderCounter = 0;

	bool m_Enabled = false;
	bool m_isRendering = false;
	bool m_isMoving = false;
	bool m_InvertedPhaseClock = false;
	bool m_UseInvertedPhaseClock = false;
};
