#include "NESMemory.hpp"

#include <string>

uint8_t NESMemory::Read(uint16_t address)
{
	if (address < 0x2000)
	{
		return m_map[address & 0x7ff];
	}
	else if (address < 0x4020)
	{
		if (address < 0x4000) //PPU registers, mirrored
		{

		}
		else if (address < 0x4018 && address >= 0x4014) //Only *some* IO registers
		{

		}
		else
		{

		}
	}
	else if (address < 0x6000) // Expansion ROM
	{

	}
	else if (address < 0x8000) // Extended RAM
	{

	}
	else //PRG
	{
		return m_mapper->ReadPRG(address);
	}

	return 0;
}

void NESMemory::Write(uint16_t address, uint8_t value)
{
	if (address < 0x2000)
	{
		m_map[address & 0x7ff] = value;
	}
	else if (address < 0x4020)
	{
		if (address < 0x4000) //PPU registers, mirrored
		{

		}
		else if (address < 0x4017 && address >= 0x4014) //only some registers
		{

		}
		else
			printf("Write access attmept at: %x\n", address);
	}
	else if (address < 0x6000)
	{
		throw std::runtime_error("Expansion ROM access attempted. This is currently unsupported\n");
	}
	else if (address < 0x8000)
	{
//		if (m_extendedRAM)
//			m_extRAM[addr - 0x6000] = value;
	}
	else
	{
		m_mapper->WritePRG(address, value);
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
	printf("Name Table Mirroring: 0x%02x\n", m_nameTableMirroring);

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

	std::vector<uint8_t> PRG_ROM(bankSize);
	memcpy(PRG_ROM.data(), pROM, bankSize);
	pROM += bankSize;

	std::vector<uint8_t> CHR_ROM;

	//CHR-ROM 8KB banks
	if (vbanks)
	{
		size_t vBankSize = 0x2000 * vbanks;
		CHR_ROM.resize(vBankSize);
		memcpy(CHR_ROM.data(), pROM, vBankSize);
	}
	else
		printf("Cartridge with CHR-RAM.\n");

	m_mapper = Mapper::Create(m_mapperNumber, std::move(PRG_ROM), std::move(CHR_ROM));

	return true;
}

uint8_t NESMemory::GetMapper() const
{
	return m_mapperNumber;
}

bool NESMemory::GetNameTableMirroring() const
{
	return m_nameTableMirroring;
}

bool NESMemory::HasExtendedRAM() const
{
	return m_extendedRAM;
}