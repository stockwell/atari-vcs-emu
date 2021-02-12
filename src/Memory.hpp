#pragma once

#include <array>
#include <memory>

#include "Common.hpp"

class MOS6502Core;

static constexpr int kMemorySize = 0xFFFF;

class Memory
{
public:
	void Reset();
	void SetProcessor(std::shared_ptr<MOS6502Core> pProcessor);

	virtual uint8_t Read(uint16_t address) = 0;
	virtual void Write(uint16_t address, uint8_t value) = 0;

	void Load(uint16_t address, uint8_t value);
	void Load(uint16_t address, uint8_t *bytes, size_t numBytes);
	virtual bool LoadROM(const uint8_t *pROM, uint16_t offset) = 0;

	void DumpRAM();

protected:
	std::shared_ptr<MOS6502Core> m_pProcessor = nullptr;
	std::array<uint8_t, kMemorySize> m_map;
};
