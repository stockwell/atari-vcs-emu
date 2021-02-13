#pragma once

#include <vector>

#include "Memory.hpp"
#include "Mapper.hpp"

class NESMemory : public Memory
{
public:
	uint8_t Read(uint16_t address) override;
	void Write(uint16_t address, uint8_t value) override;

	bool LoadROM(const uint8_t *pROM, uint16_t offset) override;

	uint8_t GetMapper() const;
	bool GetNameTableMirroring() const;
	bool HasExtendedRAM() const;

private:
	bool m_extendedRAM = false;
	uint8_t m_mapperNumber = 0;
	uint8_t m_nameTableMirroring = 0;

	std::unique_ptr<Mapper> m_mapper;
};