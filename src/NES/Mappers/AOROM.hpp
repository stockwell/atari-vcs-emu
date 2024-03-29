#pragma once

#include "NES/Mapper.hpp"

#include <cstdint>

class AOROM : public Mapper
{
public:
	AOROM(std::vector<uint8_t>* pPRG_ROM, std::vector<uint8_t>* pCHR_ROM, std::function<void(NameTableMirroring&)> mirroring_cb);
	~AOROM() override = default;

	void 	WritePRG(uint16_t address, uint8_t value) override;
	uint8_t ReadPRG(uint16_t address) override;

	void 	WriteCHR(uint16_t address, uint8_t value) override;
	uint8_t ReadCHR(uint16_t address) override;

	const uint8_t* GetPagePtr(uint16_t addr) override;

private:
	std::vector<uint8_t>*	m_pPRG_ROM;
	std::vector<uint8_t>*	m_pCHR_ROM;
	std::vector<uint8_t>	m_CHR_RAM;

	uint8_t*				m_pPRG_ROM_BANK;

	std::function<void(NameTableMirroring&)> m_mirroringCallback;
	NameTableMirroring	m_mirrorMode;

	uint16_t m_addressMask = 0x7FFF;
	bool m_usesCharacterRAM = false;
};

