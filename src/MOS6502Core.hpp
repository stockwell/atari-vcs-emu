#pragma once

#include <memory>

#include "Common.hpp"
#include "Memory.hpp"

class MOS6502Core
{
public:
	explicit MOS6502Core(std::shared_ptr<Memory> pMemory);

	void Reset();
	void InitOpcodeTable();
	void Tick();
	void Halt(uint16_t cycles);
	void Resume();
	void NMI_IRQ(bool state);
	void IRQ(bool state);
	int GetCycleCount() { return m_Cycles / 3; }
	void ClearCycleCount() { m_Cycles = 0; }

public:
	uint8_t m_Delay;
	uint8_t m_CycleTime;
	uint16_t m_PC;
	uint8_t m_AC;
	uint8_t m_XR;
	uint8_t m_YR;
	uint8_t m_SR;
	uint8_t m_SP;

public:
	/* Base memory address of the stack */
	static constexpr auto kStackBase = 0x100u;

	/* Bits in the status register */
	enum statusRegs : uint8_t
	{
		negative 	= 0x80u,
		overflow 	= 0x40u,
		constant 	= 0x20u,
		brk 		= 0x10u,
		decimal 	= 0x08u,
		interrupt 	= 0x04u,
		zero 		= 0x02u,
		carry 		= 0x01u,
	};

	/* Vectors */
	enum vectorAddresses : uint16_t
	{
		nmi = 0xFFFA,
		reset = 0xFFFC,
		irq_brq = 0xFFFE,
	};

private:
	void (MOS6502Core::*m_OPCodes[0x100])();

	bool m_pendingNMI = false;
	bool m_pendingIRQ = false;

	bool m_Running = false;
	uint16_t m_HBlankCycles = 0;
	uint32_t m_Cycles = 0;

	std::shared_ptr<Memory> m_pMemory = nullptr;

	uint8_t FetchOPCode();
	uint8_t ExecuteOPCode(uint8_t opcode);
	uint8_t ExecuteIRQ();

	void OPCodeInvalid();

	void OPCode0x00();
	void OPCode0x01();
	void OPCode0x05();
	void OPCode0x06();
	void OPCode0x08();
	void OPCode0x09();
	void OPCode0x0A();
	void OPCode0x0D();
	void OPCode0x0E();

	void OPCode0x10();
	void OPCode0x11();
	void OPCode0x15();
	void OPCode0x16();
	void OPCode0x18();
	void OPCode0x19();
	void OPCode0x1D();
	void OPCode0x1E();

	void OPCode0x20();
	void OPCode0x21();
	void OPCode0x24();
	void OPCode0x25();
	void OPCode0x26();
	void OPCode0x28();
	void OPCode0x29();
	void OPCode0x2A();
	void OPCode0x2C();
	void OPCode0x2D();
	void OPCode0x2E();

	void OPCode0x30();
	void OPCode0x31();
	void OPCode0x35();
	void OPCode0x36();
	void OPCode0x38();
	void OPCode0x39();
	void OPCode0x3D();
	void OPCode0x3E();

	void OPCode0x40();
	void OPCode0x41();
	void OPCode0x45();
	void OPCode0x46();
	void OPCode0x48();
	void OPCode0x49();
	void OPCode0x4A();
	void OPCode0x4C();
	void OPCode0x4D();
	void OPCode0x4E();

	void OPCode0x50();
	void OPCode0x51();
	void OPCode0x55();
	void OPCode0x56();
	void OPCode0x58();
	void OPCode0x59();
	void OPCode0x5D();
	void OPCode0x5E();

	void OPCode0x60();
	void OPCode0x61();
	void OPCode0x65();
	void OPCode0x66();
	void OPCode0x68();
	void OPCode0x69();
	void OPCode0x6A();
	void OPCode0x6C();
	void OPCode0x6D();
	void OPCode0x6E();

	void OPCode0x70();
	void OPCode0x71();
	void OPCode0x75();
	void OPCode0x76();
	void OPCode0x78();
	void OPCode0x79();
	void OPCode0x7D();
	void OPCode0x7E();

	void OPCode0x81();
	void OPCode0x84();
	void OPCode0x85();
	void OPCode0x86();
	void OPCode0x88();
	void OPCode0x8A();
	void OPCode0x8C();
	void OPCode0x8D();
	void OPCode0x8E();

	void OPCode0x90();
	void OPCode0x91();
	void OPCode0x94();
	void OPCode0x95();
	void OPCode0x96();
	void OPCode0x98();
	void OPCode0x99();
	void OPCode0x9A();
	void OPCode0x9D();

	void OPCode0xA0();
	void OPCode0xA1();
	void OPCode0xA2();
	void OPCode0xA4();
	void OPCode0xA5();
	void OPCode0xA6();
	void OPCode0xA8();
	void OPCode0xA9();
	void OPCode0xAA();
	void OPCode0xAC();
	void OPCode0xAD();
	void OPCode0xAE();

	void OPCode0xB0();
	void OPCode0xB1();
	void OPCode0xB4();
	void OPCode0xB5();
	void OPCode0xB6();
	void OPCode0xB8();
	void OPCode0xB9();
	void OPCode0xBA();
	void OPCode0xBC();
	void OPCode0xBD();
	void OPCode0xBE();

	void OPCode0xC0();
	void OPCode0xC1();
	void OPCode0xC4();
	void OPCode0xC5();
	void OPCode0xC6();
	void OPCode0xC8();
	void OPCode0xC9();
	void OPCode0xCA();
	void OPCode0xCC();
	void OPCode0xCD();
	void OPCode0xCE();

	void OPCode0xD0();
	void OPCode0xD1();
	void OPCode0xD5();
	void OPCode0xD6();
	void OPCode0xD8();
	void OPCode0xD9();
	void OPCode0xDD();
	void OPCode0xDE();

	void OPCode0xE0();
	void OPCode0xE1();
	void OPCode0xE4();
	void OPCode0xE5();
	void OPCode0xE6();
	void OPCode0xE8();
	void OPCode0xE9();
	void OPCode0xEA();
	void OPCode0xEC();
	void OPCode0xED();
	void OPCode0xEE();

	void OPCode0xF0();
	void OPCode0xF1();
	void OPCode0xF5();
	void OPCode0xF6();
	void OPCode0xF8();
	void OPCode0xF9();
	void OPCode0xFD();
	void OPCode0xFE();

	void OPCodesSBC(uint16_t address);
	void OPCodesADC(uint16_t address);
	void OPCodesASL(uint16_t address);
	void OPCodesCMP(uint16_t address);
	void OPCodesCPX(uint16_t address);
	void OPCodesCPY(uint16_t address);
	void OPCodesINC(uint16_t address);
	void OPCodesDEC(uint16_t address);
	void OPCodesORA(uint16_t address);
	void OPCodesLDA(uint16_t address);
	void OPCodesLDX(uint16_t address);
	void OPCodesLDY(uint16_t address);
	void OPCodesSTA(uint16_t address);
	void OPCodesSTX(uint16_t address);
	void OPCodesSTY(uint16_t address);
	void OPCodesEOR(uint16_t address);
	void OPCodesAND(uint16_t address);
	void OPCodesLSR(uint16_t address);
	void OPCodesROR(uint16_t address);
	void OPCodesROL(uint16_t address);
	void OPCodesBIT(uint16_t address);

	void BranchRelative(bool condition);
	uint8_t StackPull8();
	uint16_t StackPull16();
	void StackPush(uint8_t pushval);
	void StackPush(uint16_t pushval);
};
