#pragma once

#include <memory>

#include "Memory.h"

class TIACore;
class RIOTCore;

namespace TIAMemory
{
	constexpr int kStartAddr 	= 0x00;
	constexpr int kEndAddr		= 0x7F;
}

namespace StackMemory
{
	constexpr int kStartAddr	= 0x100;
	constexpr int kEndAddr		= 0x1FF;
}

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

class VCSMemory : public Memory
{
public:
	void SetTIA(std::shared_ptr<TIACore> pTIA);
	void SetRIOT(std::shared_ptr<RIOTCore> pRIOT);

	// Memory
	uint8_t Read(uint16_t address) override;
	void Write(uint16_t address, uint8_t value) override;

	void LoadROM(const uint8_t *pROM, uint16_t offset) override;

private:
	std::shared_ptr<TIACore>	m_pTIA 	= nullptr;
	std::shared_ptr<RIOTCore>	m_pRIOT	= nullptr;
};