#pragma once

#include <memory>

#include "Memory.hpp"

class TIACore;
class RIOTCore;


namespace RIOTMemory
{
	constexpr int kStartAddr	= 0x200;
	constexpr int kEndAddr		= 0x2FF;
}

namespace RAMMemory
{
	constexpr int kStartAddr	= 0x80;
	constexpr int kEndAddr		= 0xFF;
}

namespace ROMMemory
{
	constexpr int kStartAddr 	= 0x1000;
	constexpr int kEndAddr		= 0x1FFF;
}

class NESMemory : public Memory
{
public:
	uint8_t Read(uint16_t address) override;
	void Write(uint16_t address, uint8_t value) override;

	void LoadROM(const uint8_t *pROM, uint16_t offset) override;
};