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


#include "TIAMissile.hpp"
#include "TIACore.hpp"

#include "DrawCounterDecodes.hpp"

TIAMissile::TIAMissile(TIACore* pTIA)
: m_Decodes(DrawCounterDecodes::get().missileDecodes()[m_DecodesOffset])
, m_TIA(pTIA)
{
}

void TIAMissile::SetSize(uint8_t value)
{
	static constexpr std::array<uint8_t, 4> ourWidths = { 1, 2, 4, 8 };

	m_DecodesOffset = value & 0x07;
	m_Width = ourWidths[(value & 0x30) >> 4];
	m_Decodes = DrawCounterDecodes::get().missileDecodes()[m_DecodesOffset];

	if (m_isRendering && m_RenderCounter >= m_Width)
		m_isRendering = false;
}

void TIAMissile::ResetPos(uint8_t value, bool hblank)
{
	m_Counter = value & 0x02u;

	if (m_isRendering)
	{
		if (m_RenderCounter < 0) {
			m_RenderCounter = kRenderCounterOffset + (value - 157);

		} else {
			// The following is an effective description of the behavior of missile width after a
			// RESMx during draw. It would be much simpler without the HBLANK cases :)

			switch (m_Width)
			{
				case 8:
					m_RenderCounter = (value - 157) + ((m_RenderCounter >= 4) ? 4 : 0);
					break;

				case 4:
					m_RenderCounter = (value - 157);
					break;

				case 2:
					if (hblank)
						m_isRendering = m_RenderCounter > 1;
					else if (m_RenderCounter == 0)
						++m_RenderCounter;

					break;

				default:
					if (hblank)
						m_isRendering = m_RenderCounter > 0;
					break;
			}
		}
	}
}

void TIAMissile::ResetPosPlayer(uint8_t value, uint8_t pos)
{
	const uint8_t resmp = value & 0x02u;

	if (resmp == m_Resmp)
		return;

	m_Resmp = resmp;

	if (! m_Resmp)
		m_Counter = pos;

	UpdateEnabled();
}

void TIAMissile::MovementTick(uint8_t clock, uint8_t hclock, bool hblank)
{
	if(clock == m_HmxClocks)
		m_isMoving = false;

	if (m_isMoving)
	{
		if (hblank)
			Tick(clock, hclock);

		m_InvertedPhaseClock = ! hblank;
	}
}

void TIAMissile::Tick(uint8_t hcount, bool isReceivingMclock)
{
	m_isVisible =
		m_isRendering &&
			(m_RenderCounter >= 0 || (m_isMoving && isReceivingMclock && m_RenderCounter == -1 && m_Width < 4 && ((hcount + 1) % 4 == 3)));

	m_Collision = (m_isVisible && m_Enabled) ? kCollisionMaskEnabled : kCollisionMaskDisabled;

	if (m_Decodes[m_Counter] && ! m_Resmp)
	{
		m_isRendering = true;
		m_RenderCounter = kRenderCounterOffset;
	}
	else if (m_isRendering)
	{

		if (m_RenderCounter == -1)
		{
			if (m_isMoving && isReceivingMclock)
			{
				switch ((hcount + 1) % 4)
				{
					case 3:
						m_EffectiveWidth = m_Width == 1 ? 2 : m_Width;
						if (m_Width < 4)
							++m_RenderCounter;
						break;

					case 2:
						m_EffectiveWidth = 0;
						break;

					default:
						m_EffectiveWidth = m_Width;
						break;
				}
			}
			else
			{
				m_EffectiveWidth = m_Width;
			}
		}

		if (++m_RenderCounter >= (m_isMoving ? m_EffectiveWidth : m_Width))
			m_isRendering = false;
	}

	if (++m_Counter >= TIAConstants::H_PIXEL)
		m_Counter = 0;
}

void TIAMissile::NextLine()
{
	m_isVisible = m_isRendering && (m_RenderCounter >= 0);
	m_Collision = (m_isVisible && m_Enabled) ? kCollisionMaskEnabled : kCollisionMaskDisabled;
}

void TIAMissile::SetEnable(bool enabled)
{
	const auto oldEnam = m_Enam;

	m_Enam = enabled;

	if (oldEnam != m_Enam)
		UpdateEnabled();
}

void TIAMissile::UpdateEnabled()
{
	m_Enabled = m_Enam && ! m_Resmp;

	m_Collision = (m_isVisible && m_Enabled) ? kCollisionMaskEnabled : kCollisionMaskDisabled;
	m_TIA->ScheduleCollisionUpdate();
}