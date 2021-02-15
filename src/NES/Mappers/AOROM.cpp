#include "AOROM.hpp"

AOROM::AOROM(std::vector<uint8_t>* pPRG_ROM, std::vector<uint8_t>* pCHR_ROM)
{
	m_pPRG_ROM = pPRG_ROM;
	m_pCHR_ROM = pCHR_ROM;

	if (m_pCHR_ROM->empty())
	{
		m_usesCharacterRAM = true;
		m_CHR_RAM.resize(0x2000);
	}
}

void AOROM::WritePRG(uint16_t address, uint8_t value)
{
	if (address & 0x8000)
	{
		m_bank = value & 0x0f;
		m_mirror = value & 0x10;
	}
}

uint8_t AOROM::ReadPRG(uint16_t address)
{
	return (*m_pPRG_ROM)[(address - 0x8000) & m_addressMask];
}

const uint8_t* AOROM::GetPagePtr(uint16_t addr)
{
	return &(*m_pPRG_ROM)[(addr - 0x8000) & m_addressMask];
}

uint8_t AOROM::ReadCHR(uint16_t address)
{
	if (m_usesCharacterRAM)
		return m_CHR_RAM[address];
	else
		return (*m_pCHR_ROM)[address];
}

void AOROM::WriteCHR(uint16_t address, uint8_t value)
{
	if (m_usesCharacterRAM)
		m_CHR_RAM[address] = value;
	else
		printf("Read-only CHR memory write attempt at 0x%02x\n", address);
}

