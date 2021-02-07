#include "NESMemory.hpp"

uint8_t NESMemory::Read(uint16_t address)
{
	// TODO: Probably need something like VCS here
	uint16_t actualAddress = address;

	/* ROM Cart */
	{
		return m_map[actualAddress];
	}
}

void NESMemory::Write(uint16_t address, uint8_t value)
{
    // TODO: Probably need something like VCS here
    uint16_t actualAddress = address;

	{
		printf("Invalid write address! Addr = 0x%2X\n", address);
	}
}

void NESMemory::LoadROM(const uint8_t *pROM, uint16_t romSize)
{
	/* For a 2K ROM cart A11 isn't connected - effectively 0x1000 - 0x17FF is mirrored from 0x1800 - 0x1FFF */
	for (size_t i = ROMMemory::kStartAddr, j = 0; i < ROMMemory::kEndAddr; i++, j++)
		m_map[i] = pROM[j % romSize];
}