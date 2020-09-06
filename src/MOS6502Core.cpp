#include <iostream>

#include "MOS6502Core.hpp"
#include "Opcodes.hpp"

MOS6502Core::MOS6502Core(std::shared_ptr<Memory> pMemory)
: m_pMemory(pMemory)
{
	InitOpcodeTable();
}

void MOS6502Core::Reset()
{
	/* Load reset vector */
	m_PC = (unsigned) m_pMemory->Read(RESET_VECTOR + 1) << 8u | m_pMemory->Read(RESET_VECTOR);
	m_AC = 0x00;
	m_XR = 0x00;
	m_YR = 0x00;
	m_SR = CONSTANT;
	m_SP = 0xFD;

	m_Delay = 1;

	this->Resume();
}

uint8_t MOS6502Core::FetchOPCode()
{
	return m_pMemory->Read(m_PC);
}

uint8_t MOS6502Core::ExecuteOPCode(uint8_t opcode)
{
	m_CycleTime = 0;
	Log("Opcode: %s(0x%02X), PC 0x%04X", kOPCodeNames[opcode], opcode, m_PC);
	Log("SR: 0x%02X  |  AC: 0x%02X  |  XR: 0x%02X | YR: 0x%02X | SP: 0x%02X [ %s%s%s%s%s%s%s]",
		m_SR, m_AC, m_XR, m_YR, m_SP,
		m_SR & CARRY ? "C " : "",
		m_SR & ZERO ? "Z " : "",
		m_SR & INTERRUPT ? "I " : "",
		m_SR & DECIMAL ? "D " : "",
		m_SR & BREAK ? "B " : "",
		m_SR & OVERFLOW ? "O " : "",
		m_SR & NEGATIVE ? "N " : "");
	(this->*m_OPCodes[opcode])();

	return cycletime[opcode] + (m_CycleTime * 3);
}

void MOS6502Core::Tick()
{
	if (!m_Running)
	{
		if (m_Delay > 1)
			--m_Delay;

		return;
	}

	/* MOS6507 clock is 1/3 the graphics clock */
#ifndef DISABLE_CPU_CYCLE_ACCURACY
	if (--m_Delay)
		return;
#endif

	m_Delay = (ExecuteOPCode(FetchOPCode()));
}

void MOS6502Core::Halt()
{
	m_Running = false;
}

void MOS6502Core::Resume()
{
	m_Running = true;
}
