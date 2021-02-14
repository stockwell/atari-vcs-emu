#include "NROM.hpp"

NROM::NROM(std::vector<uint8_t>* pPRG_ROM, std::vector<uint8_t>* pCHR_ROM)
{
	m_pPRG_ROM = pPRG_ROM;
	m_pCHR_ROM = pCHR_ROM;

	if (m_pPRG_ROM->size() == 0x4000)
		m_addressMask = 0x3FFF;

	if (m_pCHR_ROM->empty())
		m_usesCharacterRAM = true;
}

void NROM::WritePRG(uint16_t address, uint8_t value)
{
	printf("PRG ROM write attempt!\n");
}

uint8_t NROM::ReadPRG(uint16_t address)
{
	return (*m_pPRG_ROM)[(address - 0x8000) & m_addressMask];
}

const uint8_t* NROM::GetPagePtr(uint16_t addr)
{
	return &(*m_pPRG_ROM)[(addr - 0x8000) & m_addressMask];
}

uint8_t NROM::ReadCHR(uint16_t address)
{
	if (m_usesCharacterRAM)
		return m_CHR_RAM[address];
	else
		return (*m_pCHR_ROM)[address];
}

void NROM::WriteCHR(uint16_t address, uint8_t value)
{
	if (m_usesCharacterRAM)
		m_CHR_RAM[address] = value;
	else
		printf("Read-only CHR memory write attempt at 0x%02x\n", address);
}

