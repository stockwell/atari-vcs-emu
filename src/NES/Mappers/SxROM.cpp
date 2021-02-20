#include "SxROM.hpp"

SxROM::SxROM(std::vector<uint8_t>* pPRG_ROM, std::vector<uint8_t>* pCHR_ROM, std::function<void(NameTableMirroring&)> mirroring_cb)
	: m_mirroringCallback(std::move(mirroring_cb))
{
	m_pPRG_ROM = pPRG_ROM;
	m_pCHR_ROM = pCHR_ROM;

	NameTableMirroring mode = NameTableMirroring::Horizontal;
	m_mirroringCallback(mode);

	if (m_pCHR_ROM->empty())
	{
		m_CHR_RAM.resize(0x2000);
	}
	else
	{
		m_firstBankCHR = &(*m_pCHR_ROM)[0];
		m_secondBankCHR = &(*m_pCHR_ROM)[0x1000 * m_regCHR1];
	}

	m_firstBankPRG = &(*m_pPRG_ROM)[0]; //first bank
	m_secondBankPRG = &(*m_pPRG_ROM)[m_pPRG_ROM->size() - 0x4000/*0x2000 * 0x0e*/]; //last bank
}

uint8_t SxROM::ReadPRG(uint16_t addr)
{
	if (addr < 0xc000)
		return *(m_firstBankPRG + (addr & kAddressMask));
	else
		return *(m_secondBankPRG + (addr & kAddressMask));
}

void SxROM::WritePRG(uint16_t addr, uint8_t value)
{
	if (!(value & 0x80)) //if reset bit is NOT set
	{
		m_tempRegister = (m_tempRegister >> 1) | ((value & 1) << 4);
		++m_writeCounter;

		if (m_writeCounter == 5)
		{
			if (addr <= 0x9fff)
			{
				NameTableMirroring mode;
				switch (m_tempRegister & 0x3)
				{
					case 0:     mode = NameTableMirroring::OneScreenLower;    break;
					case 1:     mode = NameTableMirroring::OneScreenHigher;   break;
					case 2:     mode = NameTableMirroring::Vertical;          break;
					case 3:     mode = NameTableMirroring::Horizontal;        break;
				}
				m_mirroringCallback(mode);

				m_modeCHR = (m_tempRegister & 0x10) >> 4;
				m_modePRG = (m_tempRegister & 0xc) >> 2;

				CalculatePRGPointers();

				//Recalculate CHR pointers
				if (m_modeCHR == 0) //one 8KB bank
				{
					m_firstBankCHR = &(*m_pCHR_ROM)[0x1000 * (m_regCHR0 | 1)]; //ignore last bit
					m_secondBankCHR = m_firstBankCHR + 0x1000;
				}
				else //two 4KB banks
				{
					m_firstBankCHR = &(*m_pCHR_ROM)[0x1000 * m_regCHR0];
					m_secondBankCHR = &(*m_pCHR_ROM)[0x1000 * m_regCHR1];
				}
			}
			else if (addr <= 0xbfff) //CHR Reg 0
			{
				m_regCHR0 = m_tempRegister;
				m_firstBankCHR = &(*m_pCHR_ROM)[0x1000 * (m_tempRegister | (1 - m_modeCHR))]; //OR 1 if 8KB mode

				if (m_modeCHR == 0)
					m_secondBankCHR = m_firstBankCHR + 0x1000;
			}
			else if (addr <= 0xdfff)
			{
				m_regCHR1 = m_tempRegister;

				if (m_modeCHR == 1)
					m_secondBankCHR = &(*m_pCHR_ROM)[0x1000 * m_tempRegister];
			}
			else
			{
				//TODO PRG-RAM
				if ((m_tempRegister & 0x10) == 0x10)
				{
					printf("PRG-RAM activated\n");
				}

				m_tempRegister &= 0xf;
				m_regPRG = m_tempRegister;
				CalculatePRGPointers();
			}

			m_tempRegister = 0;
			m_writeCounter = 0;
		}
	}
	else //reset
	{
		m_tempRegister = 0;
		m_writeCounter = 0;
		m_modePRG = 3;
		CalculatePRGPointers();
	}
}

void SxROM::CalculatePRGPointers()
{
	if (m_modePRG <= 1) //32KB changeable
	{
		// equivalent to multiplying 0x8000 * (m_regPRG >> 1)
		m_firstBankPRG = &(*m_pPRG_ROM)[0x4000 * (m_regPRG & ~1)];
		m_secondBankPRG = m_firstBankPRG + 0x4000;   //add 16KB
	}
	else if (m_modePRG == 2) //fix first switch second
	{
		m_firstBankPRG = &(*m_pPRG_ROM)[0];
		m_secondBankPRG = m_firstBankPRG + 0x4000 * m_regPRG;
	}
	else //switch first fix second
	{
		m_firstBankPRG = &(*m_pPRG_ROM)[0x4000 * m_regPRG];
		m_secondBankPRG = &(*m_pPRG_ROM)[m_pPRG_ROM->size() - 0x4000/*0x2000 * 0x0e*/];
	}
}

const uint8_t* SxROM::GetPagePtr(uint16_t addr)
{
	if (addr < 0xc000)
		return (m_firstBankPRG + (addr & kAddressMask));
	else
		return (m_secondBankPRG + (addr & kAddressMask));
}

uint8_t SxROM::ReadCHR(uint16_t addr)
{
	if (! m_CHR_RAM.empty())
		return m_CHR_RAM[addr];
	else if (addr < 0x1000)
		return *(m_firstBankCHR + addr);
	else
		return *(m_secondBankCHR + (addr & 0xfff));
}

void SxROM::WriteCHR(uint16_t addr, uint8_t value)
{
	if (! m_CHR_RAM.empty())
		m_CHR_RAM[addr] = value;
}


