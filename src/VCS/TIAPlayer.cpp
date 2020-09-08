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


#include "TIAPlayer.hpp"
#include "TIACore.hpp"

#include "DrawCounterDecodes.hpp"

TIAPlayer::TIAPlayer(TIACore* pTIA, uint32_t collisionMask)
: m_Decodes(DrawCounterDecodes::get().playerDecodes()[m_DecodesOffset])
, m_TIA(pTIA)
{
	SetDivider(1);

	m_CollisionMaskDisabled = collisionMask;
	m_Collision = collisionMask;
}

uint8_t TIAPlayer::GetPos()
{
	switch (m_Divider)
	{
		case 1:
			return (m_Counter + TIAConstants::H_PIXEL - 5) % TIAConstants::H_PIXEL;

		case 2:
			return (m_Counter + TIAConstants::H_PIXEL - 9) % TIAConstants::H_PIXEL;

		case 4:
			return (m_Counter + TIAConstants::H_PIXEL - 12) % TIAConstants::H_PIXEL;

		default:
			throw std::runtime_error("invalid width");
	}
}

void TIAPlayer::ResetPos(uint8_t value)
{
	m_Counter = value;

	// This tries to account for the effects of RESP during draw counter decode as
	// described in Andrew Towers' notes. Still room for tuning.'

	if (m_isRendering && (m_RenderCounter - kRenderCounterOffset) < 4)
		m_RenderCounter = kRenderCounterOffset + (value - 157);
}

void TIAPlayer::SetSize(uint8_t value, bool hblank)
{
	m_DecodesOffset = value & 0x07;

	switch (m_DecodesOffset)
	{
		case 5:
			m_DividerPending = 2;
			break;

		case 7:
			m_DividerPending = 4;
			break;

		default:
			m_DividerPending = 1;
			break;
	}

	const uint8_t* oldDecodes = m_Decodes;

	m_Decodes = DrawCounterDecodes::get().playerDecodes()[m_DecodesOffset];

	if (m_Decodes != oldDecodes &&
			m_isRendering &&
			(m_RenderCounter - kRenderCounterOffset) < 2 &&
			! m_Decodes[(m_Counter - m_RenderCounter + kRenderCounterOffset + TIAConstants::H_PIXEL - 1) % TIAConstants::H_PIXEL])
	{
		m_isRendering = false;
	}

	if (m_DividerPending == m_Divider)
		return;

	// The following is an effective description of the effects of NUSIZ during
	// decode and rendering.

	if (m_isRendering)
	{
		int8_t delta = m_RenderCounter - kRenderCounterOffset;

		switch ((m_Divider << 4) | m_DividerPending)
		{
			case 0x12:
			case 0x14:
				if (hblank)
				{
					if (delta < 4)
						SetDivider(m_DividerPending);
					else
						m_DividerChangeCounter = (delta < 5 ? 1 : 0);
				}
				else
				{
					if (delta < 3)
						SetDivider(m_DividerPending);
					else
						m_DividerChangeCounter = 1;
				}

				break;

			case 0x21:
			case 0x41:
				if (delta < (hblank ? 4 : 3))
				{
					SetDivider(m_DividerPending);
				}
				else if (delta < (hblank ? 6 : 5))
				{
					SetDivider(m_DividerPending);
					--m_RenderCounter;
				}
				else
				{
					m_DividerChangeCounter = (hblank ? 0 : 1);
				}

				break;

			case 0x42:
			case 0x24:
				if (m_RenderCounter < 1 || (hblank && (m_RenderCounter % m_Divider == 1)))
					SetDivider(m_DividerPending);
				else
					m_DividerChangeCounter = (m_Divider - (m_RenderCounter - 1) % m_Divider);
				break;

			default:
				// should never happen
				SetDivider(m_DividerPending);
				break;
		}
	}
	else
	{
		SetDivider(m_DividerPending);
	}
}

void TIAPlayer::UpdatePattern()
{
	if (m_isSuppressed)
	{
		m_Pattern = 0;
		return;
	}

	m_Pattern = m_isDelaying ? m_PatternOld : m_PatternNew;

	if (! m_Reflected)
	{
		m_Pattern = (
			((m_Pattern & 0x01) << 7) |
			((m_Pattern & 0x02) << 5) |
			((m_Pattern & 0x04) << 3) |
			((m_Pattern & 0x08) << 1) |
			((m_Pattern & 0x10) >> 1) |
			((m_Pattern & 0x20) >> 3) |
			((m_Pattern & 0x40) >> 5) |
			((m_Pattern & 0x80) >> 7)
		);
	}

	if (m_isRendering && m_RenderCounter >= m_RenderCounterTripPoint) {
		m_Collision = (m_Pattern & (1 << m_SampleCounter)) ? kCollisionMaskEnabled : m_CollisionMaskDisabled;
		m_TIA->ScheduleCollisionUpdate();
	}
}

void TIAPlayer::NextLine()
{
	if (! m_isRendering || m_RenderCounter < m_RenderCounterTripPoint)
		m_Collision = m_CollisionMaskDisabled;
	else
		m_Collision = (m_Pattern & (1 << m_SampleCounter)) ? kCollisionMaskEnabled : m_CollisionMaskDisabled;
}

void TIAPlayer::SetGraphics(uint8_t value)
{
	const uint8_t oldPatternNew = m_PatternNew;

	m_PatternNew = value;

	if (! m_isDelaying && m_PatternNew != oldPatternNew)
		UpdatePattern();

}

void TIAPlayer::SetReflected(bool value)
{
	if (value != m_Reflected)
	{
		m_Reflected = value;
		UpdatePattern();
	}
}

void TIAPlayer::SetVdelay(uint8_t value)
{
	const bool oldIsDelaying = m_isDelaying;

	m_isDelaying = (value & 0x01) > 0;

	if (oldIsDelaying != m_isDelaying)
		UpdatePattern();
}

void TIAPlayer::Tick(uint32_t x, uint32_t hcount)
{
	if(m_UseInvertedPhaseClock && m_InvertedPhaseClock)
	{
		m_InvertedPhaseClock = false;
		return;
	}

	if (! m_isRendering || m_RenderCounter < m_RenderCounterTripPoint)
		m_Collision = m_CollisionMaskDisabled;
	else
		m_Collision = (m_Pattern & (1 << m_SampleCounter)) ? kCollisionMaskEnabled : m_CollisionMaskDisabled;

	if (m_Decodes[m_Counter])
	{
		m_isRendering = true;
		m_SampleCounter = 0;
		m_RenderCounter = kRenderCounterOffset;
	}
	else if (m_isRendering)
	{
		++m_RenderCounter;

		switch (m_Divider)
		{
			case 1:
				if (m_RenderCounter > 0)
					++m_SampleCounter;

				if (m_RenderCounter >= 0 && m_DividerChangeCounter >= 0 && m_DividerChangeCounter-- == 0)
					SetDivider(m_DividerPending);

				break;

			default:
				if (m_RenderCounter > 1 && (((m_RenderCounter - 1) % m_Divider) == 0))
					++m_SampleCounter;

				if (m_RenderCounter > 0 && m_DividerChangeCounter >= 0 && m_DividerChangeCounter-- == 0)
					SetDivider(m_DividerPending);

				break;
		}

		if (m_SampleCounter > 7)
			m_isRendering = false;
	}

	if (++m_Counter >= TIAConstants::H_PIXEL)
		m_Counter = 0;
}

void TIAPlayer::MovementTick(uint32_t clock, uint8_t hclock, bool hblank)
{
	if (clock == m_HmxClocks)
		m_isMoving = false;

	if (m_isMoving)
	{
		if (hblank)
			Tick(clock, hclock);
	}
}

void TIAPlayer::SetDivider(uint8_t divider)
{
	m_Divider = divider;
	m_RenderCounterTripPoint = divider == 1 ? 0 : 1;
}

void TIAPlayer::ShufflePatterns()
{
	const uint8_t oldPatternOld = m_PatternOld;

	m_PatternOld = m_PatternNew;

	if (m_isDelaying && m_PatternOld != oldPatternOld) {
		UpdatePattern();
	}
}