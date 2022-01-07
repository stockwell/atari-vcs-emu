#include "MMC3.hpp"

MMC3::MMC3(std::vector<uint8_t>* pPRG_ROM,
		std::vector<uint8_t>* pCHR_ROM,
		std::function<void(NameTableMirroring&)> mirroring_cb,
		std::function<void(bool v)> IRQ_cb)
	: m_mirroringCallback(std::move(mirroring_cb))
	, m_IRQCallback(std::move(IRQ_cb))
	, m_PRG_RAM(0x2000)
{
	m_pPRG_ROM = pPRG_ROM;
	m_pCHR_ROM = pCHR_ROM;

	m_totalBanks = m_pPRG_ROM->size() / 0x2000;

	RefreshBankPointers();
}

void MMC3::WritePRG(uint16_t address, uint8_t value)
{
	if (address < 0x8000) //PRG RAM (if supported)
	{
		printf("WritePRG: %u\n", address);

		m_PRG_RAM[address - 0x6000] = value;

//		CPU $6000-$7FFF: 8 KB PRG RAM bank (optional)
//		CPU $8000-$9FFF (or $C000-$DFFF): 8 KB switchable PRG ROM bank
//		CPU $A000-$BFFF: 8 KB switchable PRG ROM bank
//		CPU $C000-$DFFF (or $8000-$9FFF): 8 KB PRG ROM bank, fixed to the second-last bank
//		CPU $E000-$FFFF: 8 KB PRG ROM bank, fixed to the last bank
	}
	else if (address < 0xA000) // Bank select / Bank data
	{
		if (address & 1) // Data
		{
			m_bankSel[m_bankSelectValue] = value;

			RefreshBankPointers();
		}
		else
		{
			m_bankSelectValue = value & 0x07;

			m_PRGLowerBanked = (value & 0x40) == 0x00;
			m_CHRInversion = value & 0x80;

			RefreshBankPointers();
		}
	}
	else if (address < 0xC000) // Mirroring / PRG RAM protect
	{
		if (address & 1) // PRG RAM protect
		{
			printf("PRG RAM PROT: %x\n", value);
			// 7  bit  0
			//---- ----
			//RWXX xxxx
			//||||
			//||++------ Nothing on the MMC3, see MMC6
			//|+-------- Write protection (0: allow writes; 1: deny writes)
			//+--------- PRG RAM chip enable (0: disable; 1: enable)
		}
		else // Mirroring
		{
			if (value & 1)
				m_mirrorMode = NameTableMirroring::Horizontal;
			else
				m_mirrorMode = NameTableMirroring::Vertical;

			m_mirroringCallback(m_mirrorMode);
		}
	}
	else if (address < 0xE000) // IRQ
	{
		if (address & 1)
		{
			// IRQ reload
			m_IRQReload = true;
		}
		else
		{
			// IRQ latch
			m_IRQReloadValue = value;
		}
	}
	else
	{
		bool val = address & 1;
		m_IRQEnable = val;

//		if (! val)
//			m_IRQCallback(val);
	}
}

uint8_t MMC3::ReadPRG(uint16_t address)
{
//		CPU $6000-$7FFF: 8 KB PRG RAM bank (optional)
//		CPU $8000-$9FFF (or $C000-$DFFF): 8 KB switchable PRG ROM bank
//		CPU $A000-$BFFF: 8 KB switchable PRG ROM bank
//		CPU $C000-$DFFF (or $8000-$9FFF): 8 KB PRG ROM bank, fixed to the second-last bank
//		CPU $E000-$FFFF: 8 KB PRG ROM bank, fixed to the last bank

	if (address < 0x8000) // PRG RAM (optional)
	{
		printf("PRG RAM!!!\n");

		return m_PRG_RAM[address - 0x6000];
	}
	else if (address < 0xFFFF) // PRG ROM
	{
		const uint8_t bank = (address - 0x8000) / 0x2000;
		const uint16_t offset = (address - 0x8000) - (0x2000 * bank);

		return m_pPRG_ROM_BANKS[bank][offset];
	}
}

const uint8_t* MMC3::GetPagePtr(uint16_t addr)
{
	throw std::runtime_error("blah");

	return nullptr;
}

uint8_t MMC3::ReadCHR(uint16_t address)
{
	//PPU $0000-$07FF (or $1000-$17FF): 2 KB switchable CHR bank
	//PPU $0800-$0FFF (or $1800-$1FFF): 2 KB switchable CHR bank
	//PPU $1000-$13FF (or $0000-$03FF): 1 KB switchable CHR bank
	//PPU $1400-$17FF (or $0400-$07FF): 1 KB switchable CHR bank
	//PPU $1800-$1BFF (or $0800-$0BFF): 1 KB switchable CHR bank
	//PPU $1C00-$1FFF (or $0C00-$0FFF): 1 KB switchable CHR bank

	if (m_CHRInversion)
	{
		if (address < 0x400)
		{
			return m_pCHR_ROM_BANKS[0][address];
		}
		else if (address < 0x800)
		{
			return m_pCHR_ROM_BANKS[1][address - 0x400];
		}
		else if (address < 0xC00)
		{
			return m_pCHR_ROM_BANKS[2][address - 0x800];
		}
		else if (address < 0x1000)
		{
			return m_pCHR_ROM_BANKS[3][address - 0xC00];
		}
		else if (address < 0x1800)
		{
			return m_pCHR_ROM_BANKS[4][address - 0x1000];
		}
		else
		{
			return m_pCHR_ROM_BANKS[5][address - 0x1800];
		}
	}
	else
	{
		if (address < 0x800)
		{
			return m_pCHR_ROM_BANKS[0][address];
		}
		else if (address < 0x1000)
		{
			return m_pCHR_ROM_BANKS[1][address - 0x800];
		}
		else if (address < 0x1400)
		{
			return m_pCHR_ROM_BANKS[2][address - 0x1000];
		}
		else if (address < 0x1800)
		{
			return m_pCHR_ROM_BANKS[3][address - 0x1400];
		}
		else if (address < 0x1C00)
		{
			return m_pCHR_ROM_BANKS[4][address - 0x1800];
		}
		else
		{
			return m_pCHR_ROM_BANKS[5][address - 0x1C00];
		}
	}
}

void MMC3::WriteCHR(uint16_t address, uint8_t value)
{
	printf("Read-only CHR memory write attempt at 0x%02x\n", address);
}

void MMC3::RefreshBankPointers()
{
	if (m_PRGLowerBanked)
	{
		m_pPRG_ROM_BANKS[0] = &(*m_pPRG_ROM)[0x2000 * m_bankSel[6]];
		m_pPRG_ROM_BANKS[1] = &(*m_pPRG_ROM)[0x2000 * m_bankSel[7]];
		m_pPRG_ROM_BANKS[2] = &(*m_pPRG_ROM)[0x2000 * (m_totalBanks - 2)];
		m_pPRG_ROM_BANKS[3] = &(*m_pPRG_ROM)[0x2000 * (m_totalBanks - 1)];
	}
	else
	{
		m_pPRG_ROM_BANKS[0] = &(*m_pPRG_ROM)[0x2000 * (m_totalBanks - 2)];
		m_pPRG_ROM_BANKS[1] = &(*m_pPRG_ROM)[0x2000 * m_bankSel[7]];
		m_pPRG_ROM_BANKS[2] = &(*m_pPRG_ROM)[0x2000 * m_bankSel[6]];
		m_pPRG_ROM_BANKS[3] = &(*m_pPRG_ROM)[0x2000 * (m_totalBanks - 1)];
	}

	if (m_CHRInversion)
	{
		m_pCHR_ROM_BANKS[0] = &(*m_pCHR_ROM)[0x400 * m_bankSel[2]];
		m_pCHR_ROM_BANKS[1] = &(*m_pCHR_ROM)[0x400 * m_bankSel[3]];
		m_pCHR_ROM_BANKS[2] = &(*m_pCHR_ROM)[0x400 * m_bankSel[4]];
		m_pCHR_ROM_BANKS[3] = &(*m_pCHR_ROM)[0x400 * m_bankSel[5]];
		m_pCHR_ROM_BANKS[4] = &(*m_pCHR_ROM)[0x800 * (m_bankSel[0] & 0xFE)];
		m_pCHR_ROM_BANKS[5] = &(*m_pCHR_ROM)[0x800 * (m_bankSel[1] & 0xFE)];
	}
	else
	{
		m_pCHR_ROM_BANKS[0] = &(*m_pCHR_ROM)[0x800 * (m_bankSel[0] & 0xFE)];
		m_pCHR_ROM_BANKS[1] = &(*m_pCHR_ROM)[0x800 * (m_bankSel[1] & 0xFE)];
		m_pCHR_ROM_BANKS[2] = &(*m_pCHR_ROM)[0x400 * m_bankSel[2]];
		m_pCHR_ROM_BANKS[3] = &(*m_pCHR_ROM)[0x400 * m_bankSel[3]];
		m_pCHR_ROM_BANKS[4] = &(*m_pCHR_ROM)[0x400 * m_bankSel[4]];
		m_pCHR_ROM_BANKS[5] = &(*m_pCHR_ROM)[0x400 * m_bankSel[5]];
	}
}

void MMC3::PPUA12(uint8_t value)
{
	if ( value == 1 )
	{
		if ( m_IRQCounter == 0 && m_IRQReload )
			m_IRQCounter = m_IRQReloadValue;
		else
			--m_IRQCounter;

		if ( m_IRQCounter == 0 && m_IRQEnable )
			m_IRQCallback(true);

		m_IRQReload = false;
	}
}