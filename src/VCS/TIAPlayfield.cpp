//============================================================================
//
//   SSSS    tt          lll  lll
//  SS  SS   tt           ll   ll
//  SS     tttttt  eeee   ll   ll   aaaa
//   SSSS    tt   ee  ee  ll   ll      aa
//      SS   tt   eeeeee  ll   ll   aaaaa  --  "An Atari 2600 VCS Emulator"
//  SS  SS   tt   ee      ll   ll  aa  aa
//   SSSS     ttt  eeeee llll llll  aaaaa
//
// Copyright (c) 1995-2020 by Bradford W. Mott, Stephen Anthony
// and the Stella Team
//
// See the file "License.txt" for information on usage and redistribution of
// this file, and for a DISCLAIMER OF ALL WARRANTIES.
//============================================================================


#include "TIAPlayfield.hpp"
#include "TIACore.hpp"

TIAPlayfield::TIAPlayfield(uint32_t collisionMask)
{
	m_CollisionMaskDisabled = collisionMask;
	m_Collision = collisionMask;
}

void TIAPlayfield::Tick(uint32_t x, uint32_t hcount)
{
	m_Xpos = x;

	// Reflected flag is updated only at x = 0 or x = 79
	if (m_Xpos == TIAConstants::H_PIXEL / 2 || m_Xpos == 0)
		m_Refp = m_Reflected;

	if (x & 0x03u)
		return;

	uint32_t currentPixel;

	if (m_EffectivePattern == 0) {
		currentPixel = 0;
	} else if (x < TIAConstants::H_PIXEL / 2) {
		currentPixel = m_EffectivePattern & (1 << (x >> 2));
	} else if (m_Refp) {
		currentPixel = m_EffectivePattern & (1 << (39 - (x >> 2)));
	} else {
		currentPixel = m_EffectivePattern & (1 << ((x >> 2) - 20));
	}

	m_Collision = currentPixel ? kCollisionMaskEnabled : m_CollisionMaskDisabled;
}

void TIAPlayfield::SetPF0(uint8_t val)
{
	if (m_PF0 == val >> 4)
		return;

	m_Pattern = (m_Pattern & 0x000FFFF0) | (val >> 4);
	m_PF0 = val >> 4;

	UpdatePattern();
}

void TIAPlayfield::SetPF1(uint8_t val)
{
	if (m_PF1 == val)
		return;

	m_Pattern = (m_Pattern & 0x000FF00F)
		| ((val & 0x80) >> 3)
		| ((val & 0x40) >> 1)
		| ((val & 0x20) << 1)
		| ((val & 0x10) << 3)
		| ((val & 0x08) << 5)
		| ((val & 0x04) << 7)
		| ((val & 0x02) << 9)
		| ((val & 0x01) << 11);

	m_PF1 = val;
	UpdatePattern();
}

void TIAPlayfield::SetPF2(uint8_t val)
{
	if (m_PF2 == val)
		return;


	m_Pattern = (m_Pattern & 0x00000FFF) | (val << 12);
	m_PF2 = val;

	UpdatePattern();
}

void TIAPlayfield::SetCTRL(uint8_t val)
{
	const bool reflected = (val & 0x01) > 0;
	const ColourMode colourMode = (val & 0x06) == 0x02 ? ColourMode::score : ColourMode::normal;

	if (m_Reflected == reflected && m_ColourMode == colourMode)
		return;

	m_Reflected = reflected;
	m_ColourMode = colourMode;
	ApplyColours();
}

void TIAPlayfield::ApplyColours()
{

	switch (m_ColourMode)
	{
		case ColourMode::normal:
			m_ColourLeft = m_ColourRight = m_ObjectColour &= 0xfe;
			break;

		case ColourMode::score:
			m_ColourLeft  = m_ColourP0 &= 0xfe;
			m_ColourRight = m_ColourP1 &= 0xfe;
			break;
	}
}

void TIAPlayfield::UpdatePattern()
{
	m_EffectivePattern = m_Pattern;
}

uint8_t TIAPlayfield::GetColour()
{
	return m_Xpos < TIAConstants::H_PIXEL / 2 ? m_ColourLeft : m_ColourRight;
}

void TIAPlayfield::SetColor(uint8_t colour)
{
	m_ObjectColour = colour;
	ApplyColours();
}

void TIAPlayfield::SetColourP0(uint8_t colour)
{
	m_ColourP0 = colour;
	ApplyColours();
}

void TIAPlayfield::SetColourP1(uint8_t colour)
{
	m_ColourP1 = colour;
	ApplyColours();
}