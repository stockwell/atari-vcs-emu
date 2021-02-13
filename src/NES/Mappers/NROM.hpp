#pragma once

#include "NES/Mapper.hpp"

#include <cstdint>

class NROM : public Mapper
{
public:
	NROM(std::vector<uint8_t>* pPRG_ROM, std::vector<uint8_t>* pCHR_ROM);
	~NROM() override = default;

	void 	WritePRG(uint16_t address, uint8_t value) override;
	uint8_t ReadPRG(uint16_t address) override;

	void 	WriteCHR(uint16_t address, uint8_t value) override;
	uint8_t ReadCHR(uint16_t address) override;

	const uint8_t* GetPagePtr(uint16_t addr) override;

private:
	std::vector<uint8_t>*	m_pPRG_ROM;
	std::vector<uint8_t>*	m_pCHR_ROM;
	std::vector<uint8_t>	m_CHR_RAM;

	uint16_t m_addressMask = 0xFFFF;
	bool m_usesCharacterRAM = false;
};

