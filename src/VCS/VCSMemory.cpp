#include "VCSMemory.hpp"

#include "VCS/TIACore.hpp"
#include "VCS/RIOTCore.hpp"

void VCSMemory::SetTIA(std::shared_ptr<TIACore> pTIA)
{
	m_pTIA = pTIA;
}

void VCSMemory::SetRIOT(std::shared_ptr<RIOTCore> pRIOT)
{
	m_pRIOT = pRIOT;
}

uint8_t VCSMemory::Read(uint16_t address)
{

	/* MOS 6507 doesn't have address lines 13-15 connected. Only the first 13 bits of the address actually matter */
	uint16_t actualAddress = address & 0x1FFFu;

	/* Additionally, when addressing RAM, RIOT & TIA not all address lines are connected so are mirrored on different
	 * addresses depending on what you're talking to */

	/* The TIA chip is addressed by A12=0, A7=0
	 * RAM is selected by A12=0, A9=0, A7=1
	 * RIOT is selected by A12=0, A9=1, A7=1 */

	/* TIA */
	if ((actualAddress & 0x1080u) == 0x00)
	{
		actualAddress &= 0x7Fu;
		return m_pTIA->Read(actualAddress);
	}
	/* RAM */
	else if ((actualAddress & 0x1280u) == 0x80)
	{
		return m_map[actualAddress & 0xFFu];
	}
	/* RIOT */
	else if ((actualAddress & 0x1280u) == 0x280)
	{
		actualAddress &= 0x2FFu;
		return m_map[actualAddress & 0x2FFu] = m_pRIOT->Read(actualAddress);
	}
	/* ROM Cart */
	else
	{
		return m_map[actualAddress];
	}
}

void VCSMemory::Write(uint16_t address, uint8_t value)
{
	/* MOS 6507 doesn't have address lines 13-15 connected. Only the first 13 bits of the address actually matter */
	uint16_t actualAddress = address & 0x1FFFu;

	/* Additionally, when addressing RAM, RIOT & TIA not all address lines are connected so are mirrored on different
	 * addresses depending on what you're talking to */

	/* The TIA chip is addressed by A12=0, A7=0
	 * RAM is selected by A12=0, A9=0, A7=1
	 * RIOT is selected by A12=0, A9=1, A7=1 */

	/* TIA */
	if ((actualAddress & 0x1080u) == 0x00)
	{
		actualAddress &= 0x7Fu;
		m_pTIA->Write(actualAddress, value);
	}
	/* RAM */
	else if ((actualAddress & 0x1280u) == 0x80)
	{
		m_map[actualAddress & 0xFFu] = value;
	}
	/* RIOT */
	else if ((actualAddress & 0x1280u) == 0x280)
	{
		actualAddress &= 0x2FFu;
		m_map[actualAddress] = value;
		m_pRIOT->Write(actualAddress, value);
	}
	else
	{
		printf("Invalid write address! Addr = 0x%2X\n", address);
	}
}

void VCSMemory::LoadROM(const uint8_t *pROM, uint16_t romSize)
{
	/* For a 2K ROM cart A11 isn't connected - effectively 0x1000 - 0x17FF is mirrored from 0x1800 - 0x1FFF */
	for (size_t i = ROMMemory::kStartAddr, j = 0; i < ROMMemory::kEndAddr; i++, j++)
		m_map[i] = pROM[j % romSize];
}