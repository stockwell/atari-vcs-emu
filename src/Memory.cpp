#include <iostream>
#include <cstring>

#include "Memory.h"

void Memory::SetProcessor(std::shared_ptr<MOS6502Core>pProcessor)
{
	m_pProcessor = pProcessor;
}

void Memory::Reset()
{
	for (int i = 0; i < kMemorySize; i++)
		m_map[i] = rand();
}

void Memory::Load(uint16_t address, uint8_t *bytes, size_t numBytes)
{
	/* YOLO */
	memcpy(&m_map[address & 0x1FFFu], bytes, numBytes);
}

void Memory::Load(uint16_t address, uint8_t byte)
{
	/* YOLO */
	m_map[address & 0x1FFFu] = byte;
}

void Memory::DumpRAM()
{
	hexDump("Mem", &m_map[0x80], 0x80);
}
