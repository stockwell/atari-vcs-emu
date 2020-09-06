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

#include "TIABall.hpp"
#include "TIACore.hpp"

TIABall::TIABall(TIACore* pTIA)
: m_TIA(pTIA)
{
}

void TIABall::SetEnable(bool enabled)
{
	const auto enabledNewOldValue = m_isEnabledNew;

	m_isEnabledNew = enabled;

	if (m_isEnabledNew != enabledNewOldValue && ! m_isDelaying)
	{
		UpdateEnabled();
	}
}

void TIABall::ResetPos(uint8_t value)
{
	m_Counter = value & 0x02u;

	m_isRendering = true;
	m_RenderCounter = kRenderCounterOffset + (value - 157);
}

void TIABall::ShuffleStatus()
{
	const auto oldIsEnabledOld = m_isEnabledOld;

	m_isEnabledOld = m_isEnabledNew;

	if (m_isEnabledOld != oldIsEnabledOld && m_isDelaying)
		UpdateEnabled();

}

void TIABall::UpdateEnabled()
{
	m_Enabled = m_isDelaying ? m_isEnabledOld : m_isEnabledNew;

	m_Collision = (m_SignalActive && m_Enabled) ? kCollisionMaskEnabled : kCollisionMaskDisabled;
	m_TIA->ScheduleCollisionUpdate();
}

void TIABall::SetSize(uint8_t value)
{
	static constexpr std::array<uint8_t, 4> ourWidths = { 1, 2, 4, 8 };

	const uint8_t newWidth = ourWidths[value >> 4u];

	if (newWidth != m_Width)
		m_Width = newWidth;
}

void TIABall::SetVdelay(uint8_t value)
{
	const auto oldIsDelaying = m_isDelaying;

	m_isDelaying = (value & 0x01) > 0;

	if (oldIsDelaying != m_isDelaying)
		UpdateEnabled();

}

void TIABall::NextLine()
{
	// Reevalute the collision mask in order to properly account for collisions during
	// hblank. Usually, this will be taken care off in the next tick, but there is no
	// next tick before hblank ends.
	m_SignalActive = m_isRendering && m_RenderCounter >= 0;
	m_Collision = (m_SignalActive && m_Enabled) ? kCollisionMaskEnabled : kCollisionMaskDisabled;
}

void TIABall::MovementTick(uint8_t clock, uint8_t hclock, bool hblank)
{
	m_LastMovementTick = m_Counter;

	// Stop movement once the number of clocks according to HMBL is reached
	if (clock == m_HmxClocks)
		m_isMoving = false;

	if (m_isMoving)
	{
		// Process the tick if we are in hblank. Otherwise, the tick is either masked
		// by an ordinary tick or merges two consecutive ticks into a single tick (inverted
		// movement clock phase mode).
		if (hblank)
			Tick(clock, hclock);

		// Track a tick outside hblank for later processing
		m_InvertedPhaseClock = ! hblank;
	}
}

void TIABall::Tick(uint32_t x, uint32_t hcount)
{
	// Turn on the signal if the render counter reaches the threshold
	m_SignalActive = m_isRendering && m_RenderCounter >= 0;

	// Consider enabled status and the signal to determine visibility (as represented
	// by the collision mask)
	m_Collision = (m_SignalActive && m_Enabled) ? kCollisionMaskEnabled : kCollisionMaskDisabled;

	// Regular clock pulse during movement -> starfield mode
	bool starfieldEffect = m_isMoving && m_isReceivingRegularClock;

	// Decode value that triggers rendering
	if (m_Counter == 156)
	{
		m_isRendering = true;
		m_RenderCounter = kRenderCounterOffset;

		// What follows is an effective description of ball width in starfield mode.
		uint8_t starfieldDelta = (m_Counter + TIAConstants::H_PIXEL - m_LastMovementTick) % 4;
		if (starfieldEffect && starfieldDelta == 3 && m_Width < 4) ++m_RenderCounter;

		switch (starfieldDelta) {
		case 3:
			m_EffectiveWidth = m_Width == 1 ? 2 : m_Width;
			break;

		case 2:
			m_EffectiveWidth = 0;
			break;

		default:
			m_EffectiveWidth = m_Width;
			break;
		}

	} else if (m_isRendering && ++m_RenderCounter >= (starfieldEffect ? m_EffectiveWidth : m_Width))
		m_isRendering = false;

	if (++m_Counter >= TIAConstants::H_PIXEL)
		m_Counter = 0;
}