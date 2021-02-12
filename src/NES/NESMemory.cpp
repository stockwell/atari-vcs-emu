#include "NESMemory.hpp"

#include <string>

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

bool NESMemory::LoadROM(const uint8_t *pROM, uint16_t romSize)
{
	constexpr auto kHeaderSize = 0x10;

	if (std::string{&pROM[0], &pROM[4]} != "NES\x1A")
		return false;

	printf("Parsing iNes header..\n");

	uint8_t banks = pROM[4];
	printf("16KB PRG-ROM Banks: %d\n", banks);

	if (! banks)
	{
		printf("ROM has no PRG-ROM banks. Loading ROM failed.\n");
		return false;
	}

	uint8_t vbanks = pROM[5];
	printf("8KB CHR-ROM Banks: %d\n", vbanks);

	m_nameTableMirroring = pROM[6] & 0xB;
	printf("Name Table Mirroring: %s\n", m_nameTableMirroring ? "true" : "false");

	m_mapperNumber = ((pROM[6] >> 4) & 0xf) | (pROM[7] & 0xf0);
	printf("Mapper: #%d\n", m_mapperNumber);

	m_extendedRAM = pROM[6] & 0x2;
	printf("Extended (CPU) RAM: %s\n", m_extendedRAM ? "true" : "false");

	if (pROM[6] & 0x4)
	{
		printf("Trainer is not supported.\n");
		return false;
	}

	if ((pROM[0xA] & 0x3) == 0x2 || (pROM[0xA] & 0x1))
	{
		printf("PAL ROM not supported.\n");
		return false;
	}
	else
		printf("ROM is NTSC compatible.\n");

	pROM += kHeaderSize;

	//PRG-ROM 16KB banks
	size_t bankSize = 0x4000 * banks;

	m_PRG_ROM.resize(bankSize);
	memcpy(m_PRG_ROM.data(), pROM, bankSize);
	pROM += bankSize;

	//CHR-ROM 8KB banks
	if (vbanks)
	{
		size_t vBankSize = 0x2000 * vbanks;
		m_CHR_ROM.resize(vBankSize);
		memcpy(m_CHR_ROM.data(), pROM, vBankSize);
	}
	else
		printf("Cartridge with CHR-RAM.\n");
	return true;
}