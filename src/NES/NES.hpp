#pragma once

#include <memory>
#include <vector>

#include "Common.hpp"
#include "Cart.hpp"
#include "EmulatorCore.hpp"

class MOS6502Core;
class NESMemory;

class NES : public EmulatorCore
{
public:
	NES();

	bool LoadROM(const std::vector<uint8_t>& romBuffer);
	bool LoadROM(const char *szFilePath) override;

	void RunToVBlank(std::vector<uint8_t>& framebuffer, int16_t *pSampleBuffer, int *pSampleCount) override;
	void Reset() override;
	void KeypressEvent(keypress_event_t evt, bool pressed) override;

	const framebufferInfo GetFramebufferInfo() override;
	const uint32_t* GetColourLut(size_t& lutSize) override;

private:
	std::unique_ptr<Cartridge>		m_pCartridge;
	std::shared_ptr<MOS6502Core>	m_pProcessor;
	std::shared_ptr<NESMemory>      m_pMemory;
};
