#pragma once

#include <memory>
#include <vector>

#include "Common.hpp"
#include "Cart.hpp"
#include "EmulatorCore.hpp"

class MOS6502Core;
class VCSMemory;
class TIACore;
class RIOTCore;

class AtariVCS : public EmulatorCore
{
public:
	AtariVCS();

	bool LoadROM(const std::vector<uint8_t>& romBuffer);
	bool LoadROM(const char *szFilePath) override;

	bool RunToVBlank(std::vector<uint8_t>& framebuffer, int16_t* pSampleBuffer, int* pSampleCount, int maxSampleCount) override;
	void Reset() override;
	void KeypressEvent(keypress_event_t evt, bool pressed) override;

	const framebufferInfo GetFramebufferInfo() override;
	const uint32_t* GetColourLut(size_t& lutSize) override;

private:
	std::unique_ptr<Cartridge>		m_pCartridge = nullptr;
	std::shared_ptr<MOS6502Core>	m_pProcessor = nullptr;
	std::shared_ptr<VCSMemory>		m_pMemory = nullptr;
	std::shared_ptr<TIACore>		m_pTIA = nullptr;
	std::shared_ptr<RIOTCore>		m_pRIOT = nullptr;
};
