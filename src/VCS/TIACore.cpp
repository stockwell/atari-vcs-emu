#include <algorithm>

#include "TIACore.hpp"

#include "MOS6502Core.hpp"

TIACore::TIACore(std::shared_ptr<MOS6502Core> Processor)
: m_Mem(0xFF)
, m_pProcessor(std::move(Processor))
{
	m_Background = std::make_unique<TIABase>();
	m_Player0 = std::make_unique<TIAPlayer>(this, ~CollisionMask::player0 & 0x7FFF);
	m_Player1 = std::make_unique<TIAPlayer>(this, ~CollisionMask::player1 & 0x7FFF);
	m_Missile0 = std::make_unique<TIAMissile>(this, ~CollisionMask::missile0 & 0x7FFF);
	m_Missile1 = std::make_unique<TIAMissile>(this, ~CollisionMask::missile1 & 0x7FFF);
	m_Ball = std::make_unique<TIABall>(this, ~CollisionMask::ball & 0x7FFF);
	m_Playfield = std::make_unique<TIAPlayfield>(~CollisionMask::playfield & 0x7FFF);

	m_TIAObjects.emplace_back(m_Background.get());
	m_TIAObjects.emplace_back(m_Player0.get());
	m_TIAObjects.emplace_back(m_Player1.get());
	m_TIAObjects.emplace_back(m_Missile0.get());
	m_TIAObjects.emplace_back(m_Missile1.get());
	m_TIAObjects.emplace_back(m_Ball.get());
	m_TIAObjects.emplace_back(m_Playfield.get());

	TIACore::PopulateJumpTable();
}

bool TIACore::Tick(std::vector<uint8_t>& framebuffer)
{
	// Scanlines consist of:
	// Horizontal Blank -|- Game Draw Space
	//     68 ticks      |    160 ticks
	// ------------------------------------
	//      268 ticks per scanline

	// Each full frame consists of:
	// Lines -|- Description
	//    3   | Vertical sync
	//   37   | Vertical blank
	//  192   | Game Draw Space
	//   30   | Overscan
	// -------------------------
	//      262 scanlines

	m_DelayQueue.execute
	(
		[this] (uint8_t address, uint8_t value)
		{
			DelayedWrite(address, value);
		}
	);

	m_CollisionUpdateRequired = m_CollisionUpdateScheduled;
	m_CollisionUpdateScheduled = false;

	TickMovement();

	if (m_HState == HState::blank)
		TickHblank();
	else
		TickHframe(framebuffer);

	if (m_CollisionUpdateRequired && ! m_Vblank)
		UpdateCollision();

	if (++m_Hctr >= TIAConstants::H_CLOCKS)
		NextLine();


	if (m_Vsync || (m_Vctr == 262))
	{
		m_Vctr = 0;
		m_Hctr = 0;
		m_Vsync = false;
		return true;
	}

	return false;
}

uint8_t TIACore::Read(uint16_t address)
{
	if (address < (sizeof kTIAReadRegisterNames / sizeof kTIAReadRegisterNames[0]))
		Log("TIA Read: Addr %u (%s) - %u\n", address, kTIAReadRegisterNames[address], m_Mem[address]);

	if (address <= 0x07)
		return (this->*m_ReadRegisters[address])();

	return m_Mem[address];
}

void TIACore::Write(uint16_t address, uint8_t value)
{
	if (address < (sizeof kTIAWriteRegisterNames / sizeof kTIAWriteRegisterNames[0]))
	{
		Log("TIA Write: %s (%u)\n", kTIAWriteRegisterNames[address], value);
		(this->*m_WriteRegisters[address])(value);
	}
}

void TIACore::SetTrigger(uint8_t port, bool state)
{
	switch (port)
	{
		case 0:
			m_Mem[0x0C] = state ? 0x00 : 0x80;
			break;

		case 1:
			m_Mem[0x0D] = state ? 0x00 : 0x80;
			break;
	}
}

void TIACore::RenderPixel(uint32_t x, uint32_t y, std::vector<uint8_t>& framebuffer)
{
	if (x >= TIAConstants::H_PIXEL)
		return;

	uint8_t colour = 0;

	if (! m_Vblank)
	{
		switch (m_Priority)
		{
		case Priority::pfp:  // CTRLPF D2=1, D1=ignored
			// Playfield has priority so ScoreBit isn't used
			// Priority from highest to lowest:
			//   BL/PF => P0/M0 => P1/M1 => BK
			if (m_Playfield->IsEnabled())		colour = m_Playfield->GetColour();
			else if (m_Ball->IsEnabled())		colour = m_Ball->GetColour();
			else if (m_Player0->IsEnabled())	colour = m_Player0->GetColour();
			else if (m_Missile0->IsEnabled())	colour = m_Missile0->GetColour();
			else if (m_Player1->IsEnabled())	colour = m_Player1->GetColour();
			else if (m_Missile1->IsEnabled())	colour = m_Missile1->GetColour();
			else								colour = m_Background->GetColour();
			break;

		case Priority::score:  // CTRLPF D2=0, D1=1
			// Formally we have (priority from highest to lowest)
			//   PF/P0/M0 => P1/M1 => BL => BK
			// for the first half and
			//   P0/M0 => PF/P1/M1 => BL => BK
			// for the second half. However, the first ordering is equivalent
			// to the second (PF has the same color as P0/M0), so we can just
			// write
			if (m_Player0->IsEnabled())			colour = m_Player0->GetColour();
			else if (m_Missile0->IsEnabled())	colour = m_Missile0->GetColour();
			else if (m_Playfield->IsEnabled())	colour = m_Playfield->GetColour();
			else if (m_Player1->IsEnabled())	colour = m_Player1->GetColour();
			else if (m_Missile1->IsEnabled())	colour = m_Missile1->GetColour();
			else if (m_Ball->IsEnabled())		colour = m_Ball->GetColour();
			else								colour = m_Background->GetColour();
			break;

		case Priority::normal:  // CTRLPF D2=0, D1=0
			// Priority from highest to lowest:
			//   P0/M0 => P1/M1 => BL/PF => BK
			if (m_Player0->IsEnabled())			colour = m_Player0->GetColour();
			else if (m_Missile0->IsEnabled())	colour = m_Missile0->GetColour();
			else if (m_Player1->IsEnabled())	colour = m_Player1->GetColour();
			else if (m_Missile1->IsEnabled())	colour = m_Missile1->GetColour();
			else if (m_Playfield->IsEnabled())	colour = m_Playfield->GetColour();
			else if (m_Ball->IsEnabled())		colour = m_Ball->GetColour();
			else								colour = m_Background->GetColour();
			break;
		}
	}

	int pixelIndex = y * TIAConstants::H_PIXEL + x;

	if (pixelIndex > framebuffer.size())
		return;

	framebuffer[pixelIndex] = colour;
}

void TIACore::NextLine()
{
	m_Hctr = 0;

	m_HState = HState::blank;
	m_HctrDelta = 0;

	for (const auto& obj : m_TIAObjects)
		obj->NextLine();

	++m_Vctr;

	m_pProcessor->Resume();
}

void TIACore::ScheduleCollisionUpdate()
{
	m_CollisionUpdateScheduled = true;
}

void TIACore::UpdateCollision()
{
	m_CollisionMask |= (
		m_Player0->m_Collision &
		m_Player1->m_Collision &
		m_Missile0->m_Collision &
		m_Missile1->m_Collision &
		m_Ball->m_Collision &
		m_Playfield->m_Collision
	);
}

void TIACore::TickMovement()
{
	if (! m_MovementInProgress)
		return;

	if ((m_Hctr & 0x03) == 0)
	{
		const bool hblank = m_HState == HState::blank;
		uint8_t movementCounter = m_MovementClock > 15 ? 0 : m_MovementClock;

		for (const auto& obj : m_TIAObjects)
			obj->MovementTick(movementCounter, m_Hctr, hblank);

		m_MovementInProgress =
			m_Missile0->IsMoving() ||
			m_Missile1->IsMoving() ||
			m_Player0->IsMoving()  ||
			m_Player1->IsMoving()  ||
			m_Ball->IsMoving();

		m_CollisionUpdateRequired = m_CollisionUpdateRequired || m_MovementInProgress;

		++m_MovementClock;
	}
}

void TIACore::TickHblank()
{
	switch (m_Hctr)
	{
		case 0:
			m_ExtendedHblank = false;
			break;

		case TIAConstants::H_BLANK_CLOCKS - 1:
			if (! m_ExtendedHblank)
				m_HState = HState::frame;
			break;

		case TIAConstants::H_BLANK_CLOCKS + 7:
			if (m_ExtendedHblank)
				m_HState = HState::frame;
			break;
	}

	if (m_ExtendedHblank && m_Hctr > TIAConstants::H_BLANK_CLOCKS - 1)
		m_Playfield->Tick(m_Hctr - TIAConstants::H_BLANK_CLOCKS - m_HctrDelta, m_Hctr);
}

void TIACore::TickHframe(std::vector<uint8_t>& framebuffer)
{
	const uint32_t y = m_Vctr - TIAConstants::V_BLANK_CLOCKS;
	const uint32_t x = m_Hctr - TIAConstants::H_BLANK_CLOCKS - m_HctrDelta;

	m_CollisionUpdateRequired = true;

	for (const auto& obj : m_TIAObjects)
		obj->Tick(x, m_Hctr);

	RenderPixel(x, y, framebuffer);
}

void TIACore::DelayedWrite(uint8_t address, uint8_t value)
{

	switch (address)
	{
		case 0x01:
			m_Vblank = value & 0x02u;
			break;

		case 0x2A:
			m_MovementClock = 0;
			m_MovementInProgress = true;

			if (! m_ExtendedHblank)
				m_ExtendedHblank = true;

			for (const auto& obj : m_TIAObjects)
				obj->SetHmove();

			break;

		case 0x0D:
			m_Playfield->SetPF0(value);
			break;

		case 0x0E:
			m_Playfield->SetPF1(value);
			break;

		case 0x0F:
			m_Playfield->SetPF2(value);
			break;

		case 0x09:
			m_Background->SetColor(value);
			break;

		case 0x08:
			m_Playfield->SetColor(value);
			m_Ball->SetColor(value);
			break;

		case 0x22:
			m_Missile0->SetHMx(value);
			break;

		case 0x23:
			m_Missile1->SetHMx(value);
			break;

		case 0x2B:
			for (const auto& obj : m_TIAObjects)
				obj->SetHMx(0x00);
			break;

		case 0x1B:
			m_Player0->SetGraphics(value);
			break;

		case 0x1C:
			m_Player1->SetGraphics(value);
			break;

		case 0xF0:
			m_Player0->ShufflePatterns();
			break;

		case 0xF1:
			m_Player1->ShufflePatterns();
			break;

		case 0xF2:
			m_Ball->ShuffleStatus();
			break;

		case 0x20:
			m_Player0->SetHMx(value);
			break;

		case 0x21:
			m_Player1->SetHMx(value);
			break;

		case 0x24:
			m_Ball->SetHMx(value);
			break;

		case 0x0B:
			m_Player0->SetReflected(value & 0x08);
			break;

		case 0x0C:
			m_Player1->SetReflected(value & 0x08);
			break;

		case 0x1F:
			m_Ball->SetEnable(value & 0x02u);
			break;

		case 0x1D:
			m_Missile0->SetEnable(value & 0x02u);
			break;

		case 0x1E:
			m_Missile1->SetEnable(value & 0x02u);
			break;

		default:
			break;
	}
}

void TIACore::ApplyRsync()
{
	m_HctrDelta = TIAConstants::H_CLOCKS - 3 - m_Hctr;
	m_Hctr = TIAConstants::H_CLOCKS - 3;
}

uint8_t TIACore::resxCounter()
{
	return m_HState == HState::blank ?
		   (m_Hctr >= resxLateHblankThreshold ? ResxCounter::lateHblank : ResxCounter::hblank) : ResxCounter::frame;
}

/* Cxm0p */
uint8_t TIACore::TIARead0x00()
{
	return (
		((m_CollisionMask & CollisionMask::missile0 & CollisionMask::player0) ? 0x40 : 0) |
		((m_CollisionMask & CollisionMask::missile0 & CollisionMask::player1) ? 0x80 : 0)
	) & 0b11000000;
}

/* Cxm1p */
uint8_t TIACore::TIARead0x01()
{
	return (
		((m_CollisionMask & CollisionMask::missile1 & CollisionMask::player1) ? 0x40 : 0) |
		((m_CollisionMask & CollisionMask::missile1 & CollisionMask::player0) ? 0x80 : 0)
	) & 0b11000000;
}

/* Cxp0fb */
uint8_t TIACore::TIARead0x02()
{
	return (
		((m_CollisionMask & CollisionMask::player0 & CollisionMask::ball) ? 0x40 : 0) |
		((m_CollisionMask & CollisionMask::player0 & CollisionMask::playfield) ? 0x80 : 0)
	) & 0b11000000;
}

/* Cxp1fb */
uint8_t TIACore::TIARead0x03()
{
	return (
		((m_CollisionMask & CollisionMask::player1 & CollisionMask::ball) ? 0x40 : 0) |
		((m_CollisionMask & CollisionMask::player1 & CollisionMask::playfield) ? 0x80 : 0)
	) & 0b11000000;
}

/* Cxm0fb */
uint8_t TIACore::TIARead0x04()
{
	return (
		((m_CollisionMask & CollisionMask::missile0 & CollisionMask::ball) ? 0x40 : 0) |
		((m_CollisionMask & CollisionMask::missile0 & CollisionMask::playfield) ? 0x80 : 0)
	) & 0b11000000;
}

/* Cxm1fb */
uint8_t TIACore::TIARead0x05()
{
	return (
		((m_CollisionMask & CollisionMask::missile1 & CollisionMask::ball) ? 0x40 : 0) |
		((m_CollisionMask & CollisionMask::missile1 & CollisionMask::playfield) ? 0x80 : 0)
	) & 0b11000000;
}

/* Cxblpf */
uint8_t TIACore::TIARead0x06()
{
	return (
		(m_CollisionMask & CollisionMask::ball & CollisionMask::playfield) ? 0x80 : 0
	) & 0b10000000;
}

/* Cxppmm */
uint8_t TIACore::TIARead0x07()
{
	return (
		((m_CollisionMask & CollisionMask::missile0 & CollisionMask::missile1) ? 0x40 : 0) |
		((m_CollisionMask & CollisionMask::player0 & CollisionMask::player1) ? 0x80 : 0)
	) & 0b11000000;
}

/* Vertical Sync Set-Clear              */
void TIACore::TIAWrite0x00(uint8_t value)
{
	m_Vsync = value & 0x02u;
}

/* Vertical Blank Set-Clear             */
void TIACore::TIAWrite0x01(uint8_t value)
{
	m_DelayQueue.push(0x01, value, Delay::vblank);
}

/* Wait for Horizontal blank */
void TIACore::TIAWrite0x02(uint8_t value)
{
	m_pProcessor->Halt(TIAConstants::H_CLOCKS - m_Hctr);
}

/* Rsync */
void TIACore::TIAWrite0x03(uint8_t value)
{
	ApplyRsync();
}

/* Nusiz0 */
void TIACore::TIAWrite0x04(uint8_t value)
{
	m_Player0->SetSize(value, m_HState == HState::blank);
	m_Missile0->SetSize(value);
}

/* Nusiz1 */
void TIACore::TIAWrite0x05(uint8_t value)
{
	m_Player1->SetSize(value, m_HState == HState::blank);
	m_Missile1->SetSize(value);
}

/* Colup0 */
void TIACore::TIAWrite0x06(uint8_t value)
{
	value &= 0xFE;
	m_Player0->SetColor(value);
	m_Playfield->SetColourP0(value);
	m_Missile0->SetColor(value);
}

/* Colup1 */
void TIACore::TIAWrite0x07(uint8_t value)
{
	value &= 0xFE;
	m_Player1->SetColor(value);
	m_Playfield->SetColourP1(value);
	m_Missile1->SetColor(value);
}

/* Colupf */
void TIACore::TIAWrite0x08(uint8_t value)
{
	value &= 0xFE;
	m_Playfield->SetColor(value);
	m_Ball->SetColor(value);
}

/* Colubk */
void TIACore::TIAWrite0x09(uint8_t value)
{
	m_Background->SetColor(value);
}

/* Ctrlpf */
void TIACore::TIAWrite0x0A(uint8_t value)
{
	m_Priority = (value & 0x04) ? Priority::pfp :
				 (value & 0x02) ? Priority::score : Priority::normal;

	m_Playfield->SetCTRL(value & 0x07);
	m_Ball->SetSize(value);
}

/* Refp0 */
void TIACore::TIAWrite0x0B(uint8_t value)
{
	m_DelayQueue.push(0x0B, value, Delay::refp);
}

/* Refp1 */
void TIACore::TIAWrite0x0C(uint8_t value)
{
	m_DelayQueue.push(0x0C, value, Delay::refp);
}

/* PF0 */
void TIACore::TIAWrite0x0D(uint8_t value)
{
	m_DelayQueue.push(0x0D, value, Delay::pf);
}

/* PF1 */
void TIACore::TIAWrite0x0E(uint8_t value)
{
	m_DelayQueue.push(0x0E, value, Delay::pf);
}

/* PF2 */
void TIACore::TIAWrite0x0F(uint8_t value)
{
	m_DelayQueue.push(0x0F, value, Delay::pf);
}

/* Resp0 */
void TIACore::TIAWrite0x10(uint8_t value)
{
	m_Player0->ResetPos(resxCounter());
}

/* Resp1 */
void TIACore::TIAWrite0x11(uint8_t value)
{
	m_Player1->ResetPos(resxCounter());
}

/* Resm0 */
void TIACore::TIAWrite0x12(uint8_t value)
{
	m_Missile0->ResetPos(resxCounter(), m_HState == HState::blank);
}

/* Resm1 */
void TIACore::TIAWrite0x13(uint8_t value)
{
	m_Missile1->ResetPos(resxCounter(), m_HState == HState::blank);
}

/* Resbl */
void TIACore::TIAWrite0x14(uint8_t value)
{
	m_Ball->ResetPos(resxCounter());
}

/* Audc0 */
void TIACore::TIAWrite0x15(uint8_t value)
{

}

/* Audc0 */
void TIACore::TIAWrite0x16(uint8_t value)
{

}

/* Audf0 */
void TIACore::TIAWrite0x17(uint8_t value)
{

}

/* Audf1 */
void TIACore::TIAWrite0x18(uint8_t value)
{

}

/* Audv0 */
void TIACore::TIAWrite0x19(uint8_t value)
{

}

/* Audv1 */
void TIACore::TIAWrite0x1A(uint8_t value)
{

}

/* Grp0 */
void TIACore::TIAWrite0x1B(uint8_t value)
{
	m_DelayQueue.push(0x1B, value, Delay::grp);
	m_DelayQueue.push(0xF1, 0, Delay::shufflePlayer);
}

/* Grp1 */
void TIACore::TIAWrite0x1C(uint8_t value)
{
	m_DelayQueue.push(0x1C, value, Delay::grp);
	m_DelayQueue.push(0xF0, 0, Delay::shufflePlayer);
	m_DelayQueue.push(0xF2, 0, Delay::shuffleBall);
}

/* Enam0 */
void TIACore::TIAWrite0x1D(uint8_t value)
{
	m_DelayQueue.push(0x1D, value & 0x02u, Delay::enam);
}

/* Enam1 */
void TIACore::TIAWrite0x1E(uint8_t value)
{
	m_DelayQueue.push(0x1E, value & 0x02u, Delay::enam);
}

/* Enabl */
void TIACore::TIAWrite0x1F(uint8_t value)
{
	m_DelayQueue.push(0x1F, value, Delay::enabl);
}

/* Hmp0 */
void TIACore::TIAWrite0x20(uint8_t value)
{
	m_DelayQueue.push(0x20, value, Delay::hmp);
}

/* Hmp1 */
void TIACore::TIAWrite0x21(uint8_t value)
{
	m_DelayQueue.push(0x21, value, Delay::hmp);
}

/* Hmm0 */
void TIACore::TIAWrite0x22(uint8_t value)
{
	m_DelayQueue.push(0x22, value, Delay::hmm);
}

/* Hmm1 */
void TIACore::TIAWrite0x23(uint8_t value)
{
	m_DelayQueue.push(0x23, value, Delay::hmm);
}

/* Hmbl */
void TIACore::TIAWrite0x24(uint8_t value)
{
	m_DelayQueue.push(0x24, value, Delay::hmbl);
}

/* Vdelp0 */
void TIACore::TIAWrite0x25(uint8_t value)
{
	m_Player0->SetVdelay(value);
}

/* Vdelp1 */
void TIACore::TIAWrite0x26(uint8_t value)
{
	m_Player1->SetVdelay(value);
}

/* Vdelbl */
void TIACore::TIAWrite0x27(uint8_t value)
{
	m_Ball->SetVdelay(value);
}

/* Resmp0 */
void TIACore::TIAWrite0x28(uint8_t value)
{
	m_Missile0->ResetPosPlayer(value, m_Player0->GetPos());
}

/* Resmp1 */
void TIACore::TIAWrite0x29(uint8_t value)
{
	m_Missile0->ResetPosPlayer(value, m_Player0->GetPos());
}

/* Hmove */
void TIACore::TIAWrite0x2A(uint8_t value)
{
	m_DelayQueue.push(0x2A, value, Delay::hmove);
}

/* Hmclr */
void TIACore::TIAWrite0x2B(uint8_t value)
{
	m_DelayQueue.push(0x2B, value, Delay::hmclr);
}

/* Cxclr */
void TIACore::TIAWrite0x2C(uint8_t value)
{
	m_CollisionMask = 0;
}

void TIACore::PopulateJumpTable()
{
	m_ReadRegisters[0x00] = &TIACore::TIARead0x00;
	m_ReadRegisters[0x01] = &TIACore::TIARead0x01;
	m_ReadRegisters[0x02] = &TIACore::TIARead0x02;
	m_ReadRegisters[0x03] = &TIACore::TIARead0x03;
	m_ReadRegisters[0x04] = &TIACore::TIARead0x04;
	m_ReadRegisters[0x05] = &TIACore::TIARead0x05;
	m_ReadRegisters[0x06] = &TIACore::TIARead0x06;
	m_ReadRegisters[0x07] = &TIACore::TIARead0x07;

	m_WriteRegisters[0x00] = &TIACore::TIAWrite0x00;
	m_WriteRegisters[0x01] = &TIACore::TIAWrite0x01;
	m_WriteRegisters[0x02] = &TIACore::TIAWrite0x02;
	m_WriteRegisters[0x03] = &TIACore::TIAWrite0x03;
	m_WriteRegisters[0x04] = &TIACore::TIAWrite0x04;
	m_WriteRegisters[0x05] = &TIACore::TIAWrite0x05;
	m_WriteRegisters[0x06] = &TIACore::TIAWrite0x06;
	m_WriteRegisters[0x07] = &TIACore::TIAWrite0x07;
	m_WriteRegisters[0x08] = &TIACore::TIAWrite0x08;
	m_WriteRegisters[0x09] = &TIACore::TIAWrite0x09;
	m_WriteRegisters[0x0A] = &TIACore::TIAWrite0x0A;
	m_WriteRegisters[0x0B] = &TIACore::TIAWrite0x0B;
	m_WriteRegisters[0x0C] = &TIACore::TIAWrite0x0C;
	m_WriteRegisters[0x0D] = &TIACore::TIAWrite0x0D;
	m_WriteRegisters[0x0E] = &TIACore::TIAWrite0x0E;
	m_WriteRegisters[0x0F] = &TIACore::TIAWrite0x0F;

	m_WriteRegisters[0x10] = &TIACore::TIAWrite0x10;
	m_WriteRegisters[0x11] = &TIACore::TIAWrite0x11;
	m_WriteRegisters[0x12] = &TIACore::TIAWrite0x12;
	m_WriteRegisters[0x13] = &TIACore::TIAWrite0x13;
	m_WriteRegisters[0x14] = &TIACore::TIAWrite0x14;
	m_WriteRegisters[0x15] = &TIACore::TIAWrite0x15;
	m_WriteRegisters[0x16] = &TIACore::TIAWrite0x16;
	m_WriteRegisters[0x17] = &TIACore::TIAWrite0x17;
	m_WriteRegisters[0x18] = &TIACore::TIAWrite0x18;
	m_WriteRegisters[0x19] = &TIACore::TIAWrite0x19;
	m_WriteRegisters[0x1A] = &TIACore::TIAWrite0x1A;
	m_WriteRegisters[0x1B] = &TIACore::TIAWrite0x1B;
	m_WriteRegisters[0x1C] = &TIACore::TIAWrite0x1C;
	m_WriteRegisters[0x1D] = &TIACore::TIAWrite0x1D;
	m_WriteRegisters[0x1E] = &TIACore::TIAWrite0x1E;
	m_WriteRegisters[0x1F] = &TIACore::TIAWrite0x1F;

	m_WriteRegisters[0x20] = &TIACore::TIAWrite0x20;
	m_WriteRegisters[0x21] = &TIACore::TIAWrite0x21;
	m_WriteRegisters[0x22] = &TIACore::TIAWrite0x22;
	m_WriteRegisters[0x23] = &TIACore::TIAWrite0x23;
	m_WriteRegisters[0x24] = &TIACore::TIAWrite0x24;
	m_WriteRegisters[0x25] = &TIACore::TIAWrite0x25;
	m_WriteRegisters[0x26] = &TIACore::TIAWrite0x26;
	m_WriteRegisters[0x27] = &TIACore::TIAWrite0x27;
	m_WriteRegisters[0x28] = &TIACore::TIAWrite0x28;
	m_WriteRegisters[0x29] = &TIACore::TIAWrite0x29;
	m_WriteRegisters[0x2A] = &TIACore::TIAWrite0x2A;
	m_WriteRegisters[0x2B] = &TIACore::TIAWrite0x2B;
	m_WriteRegisters[0x2C] = &TIACore::TIAWrite0x2C;
}