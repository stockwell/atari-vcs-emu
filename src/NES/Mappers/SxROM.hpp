#pragma once

#include "NES/Mapper.hpp"

#include <cstdint>
#include <functional>

class SxROM : public Mapper
{
public:
	SxROM(std::vector<uint8_t>* pPRG_ROM, std::vector<uint8_t>* pCHR_ROM, std::function<void(NameTableMirroring&)> mirroring_cb);
	~SxROM() override = default;

	void 	WritePRG(uint16_t address, uint8_t value) override;
	uint8_t ReadPRG(uint16_t address) override;

	void 	WriteCHR(uint16_t address, uint8_t value) override;
	uint8_t ReadCHR(uint16_t address) override;

	const uint8_t* GetPagePtr(uint16_t addr) override;

	void 	CalculatePRGPointers();

private:
	std::vector<uint8_t>*	m_pPRG_ROM;
	std::vector<uint8_t>*	m_pCHR_ROM;
	std::vector<uint8_t>	m_CHR_RAM;

	std::function<void(NameTableMirroring&)> m_mirroringCallback;

	static constexpr auto kAddressMask = 0x3FFF;

	uint8_t m_regPRG 		= 0;
	uint8_t m_regCHR0 		= 0;
	uint8_t m_regCHR1 		= 0;
	uint8_t m_tempRegister	= 0;

	int16_t m_modeCHR 		= 0;
	int16_t m_modePRG 		= 3;
	int16_t m_writeCounter	= 0;

	const uint8_t* m_firstBankPRG;
	const uint8_t* m_secondBankPRG;

	const uint8_t* m_firstBankCHR;
	const uint8_t* m_secondBankCHR;
};

