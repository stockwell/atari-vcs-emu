#pragma once

#include <memory>
#include <vector>

#include "Common.hpp"
#include "Cart.hpp"
#include "Controller.hpp"
#include "EmulatorCore.hpp"
#include "PictureBus.hpp"
#include "PPU.hpp"
#include "APU/Nes_Apu.h"

class MOS6502Core;
class NESMemory;

class NES : public EmulatorCore
{
public:
	NES();

	void DMA(uint8_t page);
	bool LoadROM(const std::vector<uint8_t>& romBuffer);
	bool LoadROM(const char *szFilePath) override;

	bool RunToVBlank(std::vector<uint8_t>& framebuffer, int16_t* pSampleBuffer, int* pSampleCount, int maxSampleCount) override;
	void Reset() override;
	void KeypressEvent(keypress_event_t evt, bool pressed) override;

	const framebufferInfo GetFramebufferInfo() override;
	const uint32_t* GetColourLut(size_t& lutSize) override;

private:
	std::unique_ptr<Cartridge>		m_pCartridge;
	std::shared_ptr<MOS6502Core>	m_pProcessor;
	std::shared_ptr<NESMemory>		m_pMemory;
	std::unique_ptr<PPU>			m_pPPU;
	std::shared_ptr<PictureBus>		m_pPictureBus;
	std::shared_ptr<Mapper>			m_pMapper;
	std::unique_ptr<Controller>		m_pController1;
	std::unique_ptr<Controller>		m_pController2;
	std::unique_ptr<Nes_Apu>		m_pAPU;
	std::unique_ptr<Blip_Buffer>	m_pBlipBuffer;
};
