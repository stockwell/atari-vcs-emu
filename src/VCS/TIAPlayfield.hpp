#pragma once

#include "TIABase.hpp"

class TIAPlayfield : public TIABase
{
public:
	TIAPlayfield(uint32_t collisionMask);

	void SetPF0(uint8_t val);
	void SetPF1(uint8_t val);
	void SetPF2(uint8_t val);
	void SetCTRL(uint8_t val);

	void SetColourP0(uint8_t colour);
	void SetColourP1(uint8_t colour);

	void UpdatePattern();
	void ApplyColours();

	// TIABase
	void Tick(uint32_t x, uint32_t hcount) override;
	uint8_t GetColour() override;
	void SetColor(uint8_t colour) override;

private:
	enum class ColourMode: uint8_t
	{
		normal,
		score,
	};

	ColourMode m_ColourMode = ColourMode::normal;

	uint8_t m_PF0 = 0;
	uint8_t m_PF1 = 0;
	uint8_t m_PF2 = 0;

	uint8_t m_ColourP0 = 0;
	uint8_t m_ColourP1 = 0;
	uint8_t m_ColourLeft = 0;
	uint8_t m_ColourRight = 0;
	uint8_t m_ObjectColour = 0;

	uint32_t m_Xpos = 0;
	uint32_t m_Pattern = 0;
	uint32_t m_EffectivePattern = 0;

	bool m_Reflected = false;
	bool m_Refp = false;
};