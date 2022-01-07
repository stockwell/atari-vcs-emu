#pragma once

#include "NES/Mapper.hpp"

#include <cstdint>

class MMC3 : public Mapper
{
public:
	MMC3(std::vector<uint8_t>* pPRG_ROM,
			std::vector<uint8_t>* pCHR_ROM,
			std::function<void(NameTableMirroring&)> mirroring_cb,
			std::function<void(bool v)> IRQ_cb);

	~MMC3() override = default;

	void 	WritePRG(uint16_t address, uint8_t value) override;
	uint8_t ReadPRG(uint16_t address) override;

	void 	WriteCHR(uint16_t address, uint8_t value) override;
	uint8_t ReadCHR(uint16_t address) override;

	const uint8_t* GetPagePtr(uint16_t addr) override;

	void 	PPUA12(uint8_t value) override;

private:
	void 	RefreshBankPointers();

	std::vector<uint8_t>*	m_pPRG_ROM;
	std::vector<uint8_t>*	m_pCHR_ROM;
	std::vector<uint8_t>	m_PRG_RAM;

	uint8_t 				m_totalBanks = 0;

	std::array<uint8_t, 8>  m_bankSel;
	std::array<uint8_t*, 4>	m_pPRG_ROM_BANKS;
	std::array<uint8_t*, 6>	m_pCHR_ROM_BANKS;

	bool m_CHRInversion = false;
	bool m_PRGLowerBanked = false;

	std::function<void(NameTableMirroring&)> m_mirroringCallback;
	NameTableMirroring	m_mirrorMode;

	std::function<void(bool v)> m_IRQCallback;

	uint8_t m_bankSelectValue = 0;
	uint8_t m_IRQCounter = 0;
	uint8_t m_IRQReloadValue = 0;

	bool m_IRQEnable = false;
	bool m_IRQReload = false;
};

