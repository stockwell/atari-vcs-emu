#pragma once

#include <vector>

#include "Memory.hpp"

class NESMemory : public Memory
{
public:
	uint8_t Read(uint16_t address) override;
	void Write(uint16_t address, uint8_t value) override;

	bool LoadROM(const uint8_t *pROM, uint16_t offset) override;

private:
	uint8_t m_mapperNumber = 0;
	bool m_extendedRAM = false;
	bool m_nameTableMirroring = false;

	std::vector<uint8_t> m_PRG_ROM;
	std::vector<uint8_t> m_CHR_ROM;
};