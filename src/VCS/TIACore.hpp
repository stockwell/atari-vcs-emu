#pragma once

#include <cstring>
#include <iostream>
#include <memory>
#include <vector>

#include "Common.hpp"

#include "TIAMissile.hpp"
#include "TIAPlayer.hpp"
#include "TIABall.hpp"
#include "TIAPlayfield.hpp"

class MOS6502Core;

class TIACore
{
public:
	explicit TIACore(std::shared_ptr<MOS6502Core> Processor);

	void Write(uint16_t address, uint8_t value);
	uint8_t Read(uint16_t address);

	bool Tick(std::vector<uint8_t>& framebuffer);

	void SetTrigger(uint8_t port, bool state);

private:
	void PopulateJumpTable();
	void TIAClearCollisions();

	void RenderPixel(uint32_t x, uint32_t y, std::vector<uint8_t>& framebuffer);

	void NextLine();
	void TickMovement();
	void TickHblank();
	void TickHframe(std::vector<uint8_t>& framebuffer);
	void ApplyRsync() { };

private:
	enum class HState
	{
		blank,
		frame,
	};

	enum class Priority
	{
		pfp,
		score,
		normal,
	};

	void (TIACore::*m_WriteRegisters[0x2E])(uint8_t value);
	std::vector<uint8_t> m_Mem;
	std::shared_ptr<MOS6502Core> m_pProcessor = nullptr;

	bool m_Vsync 	= false;
	bool m_Vblank 	= false;
	bool m_Hblank 	= false;
	bool m_ExtendedHblank = false;

	bool m_MovementInProgress = false;

	uint8_t m_Hctr = 0;
	int32_t m_HctrDelta = 0;

	uint16_t m_Vctr = 0;
	uint16_t m_Clock = 0;
	uint16_t m_PixelIndex = 0;

	uint32_t m_MovementClock = 0;

	HState m_HState = HState::blank;
	Priority m_Priority = Priority::normal;

	std::unique_ptr<TIABase> m_Background 		= nullptr;
	std::shared_ptr<TIAPlayer> m_Player0 		= nullptr;
	std::shared_ptr<TIAPlayer> m_Player1 		= nullptr;
	std::unique_ptr<TIAMissile> m_Missile0 		= nullptr;
	std::unique_ptr<TIAMissile> m_Missile1 		= nullptr;
	std::unique_ptr<TIABall> m_Ball 			= nullptr;
	std::unique_ptr<TIAPlayfield> m_Playfield 	= nullptr;

	std::vector<TIABase*> m_TIAObjects;

private:
	void TIAWrite0x00(uint8_t value);
	void TIAWrite0x01(uint8_t value);
	void TIAWrite0x02(uint8_t value);
	void TIAWrite0x03(uint8_t value);
	void TIAWrite0x04(uint8_t value);
	void TIAWrite0x05(uint8_t value);
	void TIAWrite0x06(uint8_t value);
	void TIAWrite0x07(uint8_t value);
	void TIAWrite0x08(uint8_t value);
	void TIAWrite0x09(uint8_t value);
	void TIAWrite0x0A(uint8_t value);
	void TIAWrite0x0B(uint8_t value);
	void TIAWrite0x0C(uint8_t value);
	void TIAWrite0x0D(uint8_t value);
	void TIAWrite0x0E(uint8_t value);
	void TIAWrite0x0F(uint8_t value);

	void TIAWrite0x10(uint8_t value);
	void TIAWrite0x11(uint8_t value);
	void TIAWrite0x12(uint8_t value);
	void TIAWrite0x13(uint8_t value);
	void TIAWrite0x14(uint8_t value);
	void TIAWrite0x15(uint8_t value);
	void TIAWrite0x16(uint8_t value);
	void TIAWrite0x17(uint8_t value);
	void TIAWrite0x18(uint8_t value);
	void TIAWrite0x19(uint8_t value);
	void TIAWrite0x1A(uint8_t value);
	void TIAWrite0x1B(uint8_t value);
	void TIAWrite0x1C(uint8_t value);
	void TIAWrite0x1D(uint8_t value);
	void TIAWrite0x1E(uint8_t value);
	void TIAWrite0x1F(uint8_t value);

	void TIAWrite0x20(uint8_t value);
	void TIAWrite0x21(uint8_t value);
	void TIAWrite0x22(uint8_t value);
	void TIAWrite0x23(uint8_t value);
	void TIAWrite0x24(uint8_t value);
	void TIAWrite0x25(uint8_t value);
	void TIAWrite0x26(uint8_t value);
	void TIAWrite0x27(uint8_t value);
	void TIAWrite0x28(uint8_t value);
	void TIAWrite0x29(uint8_t value);
	void TIAWrite0x2A(uint8_t value);
	void TIAWrite0x2B(uint8_t value);
	void TIAWrite0x2C(uint8_t value);
};

namespace TIAConstants
{
	static constexpr uint32_t frameBufferWidth = 160;
	static constexpr uint32_t frameBufferHeight = 320;
	static constexpr int32_t  minVcenter = -20; // limit to reasonable values
	static constexpr int32_t  maxVcenter = 20; // limit to reasonable values
	static constexpr uint32_t viewableWidth = 320;
	static constexpr uint32_t viewableHeight = 240;
	static constexpr uint32_t initialGarbageFrames = 10;

	static constexpr uint16_t H_PIXEL = 160;
	static constexpr uint16_t H_CYCLES = 76;
	static constexpr uint16_t CYCLE_CLOCKS = 3;
	static constexpr uint16_t H_CLOCKS = H_CYCLES * CYCLE_CLOCKS;   // = 228
	static constexpr uint16_t H_BLANK_CLOCKS = H_CLOCKS - H_PIXEL;  // = 68
}

static const char *kTIAReadRegisterNames[0x0E] = {
	"Cxm0p",    /* Read Collision M0-P1   M0-P0 */
	"Cxm0p",    /*                M1-P0   M1-P1 */
	"Cxp0fb",   /*                P0-PF   P0-BL */
	"Cxp1fb",   /*                P1-PF   P1-BL */
	"Cxm0fb",   /*                M0-PF   M0-BL */
	"Cxm1fb",   /*                M1-PF   M1-BL */
	"Cxblpf",   /*                BL-PF   ----- */
	"Cxppmm",   /*                P0-P1   M0-M1 */
	"Intp0",    /* Read Pot Port 0 */
	"Intp1",    /* Read Pot Port 1 */
	"Intp2",    /* Read Pot Port 2 */
	"Intp3",    /* Read Pot Port 3 */
	"Intp4",    /* Read Input (Trigger) 0 */
	"Intp5"     /* Read Input (Trigger) 1 */
};

static const char *kTIAWriteRegisterNames[0x2D] = {
	/* 0x00 */
	"Vsync",  /* Vertical Sync Set-Clear              */
	"Vblank", /* Vertical Blank Set-Clear             */
	"Wsync",  /* Wait for Horizontal Blank            */
	"Rsync",  /* Reset Horizontal Sync Counter        */
	"Nusiz0", /* Number-Size player/missle 0          */
	"Nusiz1", /* Number-Size player/missle 1          */
	"Colup0", /* Color-Luminance Player 0             */
	"Colup1", /* Color-Luminance Player 1             */

	/* 0x08 */
	"Colupf", /* Color-Luminance Playfield            */
	"Colubk", /* Color-Luminance Background           */
	"Ctrlpf", /* Control Playfield, Ball, Collisions  */
	"Refp0",  /* Reflection Player 0                  */
	"Refp1",  /* Reflection Player 1                  */
	"Pf0",    /* Playfield Register Byte 0            */
	"Pf1",    /* Playfield Register Byte 1            */
	"Pf2",    /* Playfield Register Byte 2            */

	/* 0x10 */
	"Resp0",  /* Reset Player 0                       */
	"Resp1",  /* Reset Player 1                       */
	"Resm0",  /* Reset Missile 0                       */
	"Resm1",  /* Reset Missile 1                       */
	"Resbl",  /* Reset Ball                           */
	"Audc0",  /* Audio Control 0                      */
	"Audc1",  /* Audio Control 1                      */
	"Audf0",  /* Audio Frequency 0                    */

	/* 0x18 */
	"Audf1",  /* Audio Frequency 1                    */
	"Audv0",  /* Audio Volume 0                       */
	"Audv1",  /* Audio Volume 1                       */
	"Grp0",   /* Graphics Register Player 0           */
	"Grp1",   /* Graphics Register Player 1           */
	"Enam0",  /* Graphics Enable Missle 0             */
	"Enam1",  /* Graphics Enable Missle 1             */
	"Enabl",  /* Graphics Enable Ball                 */

	/* 0x20 */
	"Hmp0",   /* Horizontal Motion Player 0           */
	"Hmp1",   /* Horizontal Motion Player 1           */
	"Hmm0",   /* Horizontal Motion Missle 0           */
	"Hmm1",   /* Horizontal Motion Missle 1           */
	"Hmbl",   /* Horizontal Motion Ball               */
	"Vdelp0", /* Vertical Delay Player 0              */
	"Vdelp1", /* Vertical Delay Player 1              */
	"Vdelbl", /* Vertical Delay Ball                  */

	/* 0x28 */
	"Resmp0", /* Reset Missle 0 to Player 0           */
	"Resmp1", /* Reset Missle 1 to Player 1           */
	"Hmove",  /* Apply Horizontal Motion              */
	"Hmclr",  /* Clear Horizontal Move Registers      */
	"Cxclr"   /* Clear Collision Latches              */
};