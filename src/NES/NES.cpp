#include "Common.hpp"

#include "NES.hpp"
#include "Cart.hpp"
#include "NESMemory.hpp"
#include "MOS6502Core.hpp"

NES::NES()
{
	m_pCartridge = std::make_unique<Cartridge>();
	m_pMemory = std::make_shared<NESMemory>();
	m_pProcessor = std::make_shared<MOS6502Core>(m_pMemory);
	m_pPictureBus = std::make_shared<PictureBus>();
	m_pPPU = std::make_unique<PPU>(m_pPictureBus);
	m_pController1 = std::make_unique<Controller>();
	m_pController2 = std::make_unique<Controller>();
}

bool NES::LoadROM(const char *szFilePath)
{
	if (! m_pCartridge->LoadFromFile(szFilePath))
		return false;

	m_pMemory->LoadROM(m_pCartridge->GetROM(), m_pCartridge->GetRomSize());

	return true;
}

bool NES::LoadROM(const std::vector<uint8_t> &romBuffer)
{
	if (! m_pCartridge->LoadFromBuffer(romBuffer.data(), romBuffer.size()))
		return false;

	m_pMemory->LoadROM(m_pCartridge->GetROM(), m_pCartridge->GetRomSize());
	return true;
}

void NES::Reset()
{
	m_pMapper = Mapper::Create(m_pMemory, [&](NameTableMirroring& m) { return m_pPictureBus->UpdateMirroring(m); });

	m_pPictureBus->SetMapper(m_pMapper, m_pMemory->GetNameTableMirroring());

	m_pPPU->reset();

	m_pMemory->SetMapper(m_pMapper);
	m_pMemory->SetProcessor(m_pProcessor);
	m_pMemory->Reset();

	m_pMemory->SetReadCallback(NESMemory::PPUSTATUS,	[&]() { return m_pPPU->getStatus(); });
	m_pMemory->SetReadCallback(NESMemory::PPUDATA,		[&]() { return m_pPPU->getData(); });
	m_pMemory->SetReadCallback(NESMemory::JOY1, 		[&]() { return m_pController1->Read(); });
	m_pMemory->SetReadCallback(NESMemory::JOY2, 		[&]() { return m_pController2->Read(); });
	m_pMemory->SetReadCallback(NESMemory::OAMDATA,		[&]() { return m_pPPU->getOAMData(); });


	m_pMemory->SetWriteCallback(NESMemory::PPUCTRL,	[&](uint8_t b) { m_pPPU->control(b);});
	m_pMemory->SetWriteCallback(NESMemory::PPUMASK,	[&](uint8_t b) { m_pPPU->setMask(b);});
	m_pMemory->SetWriteCallback(NESMemory::OAMADDR,	[&](uint8_t b) { m_pPPU->setOAMAddress(b);});
	m_pMemory->SetWriteCallback(NESMemory::PPUADDR,	[&](uint8_t b) { m_pPPU->setDataAddress(b);});
	m_pMemory->SetWriteCallback(NESMemory::PPUSCROL,	[&](uint8_t b) { m_pPPU->setScroll(b);});
	m_pMemory->SetWriteCallback(NESMemory::PPUDATA,	[&](uint8_t b) { m_pPPU->setData(b);});
	m_pMemory->SetWriteCallback(NESMemory::OAMDMA,		[&](uint8_t b) { DMA(b);});
	m_pMemory->SetWriteCallback(NESMemory::JOY1,		[&](uint8_t b) { m_pController1->Strobe(b); m_pController2->Strobe(b);});
	m_pMemory->SetWriteCallback(NESMemory::OAMDATA,	[&](uint8_t b) { m_pPPU->setOAMData(b);});

	m_pPPU->SetInterruptCallback([&](){ m_pProcessor->NMI_IRQ(); });

	m_pProcessor->Reset();
}

void NES::RunToVBlank(std::vector<uint8_t>& framebuffer, int16_t* pSampleBuffer, int* pSampleCount)
{
	bool frameComplete;

	do
	{
		m_pProcessor->Tick();
		frameComplete = m_pPPU->Tick(framebuffer);
	} while (! frameComplete);
}

void NES::KeypressEvent(keypress_event_t evt, bool pressed)
{
	m_pController1->HandleKeypress(evt, pressed);
}

const EmulatorCore::framebufferInfo NES::GetFramebufferInfo()
{
	static constexpr EmulatorCore::framebufferInfo info =
	{
		.width	= 256,
		.height	= 240,
	};

	return info;
}

const uint32_t *NES::GetColourLut(size_t &lutSize) {

	static constexpr uint32_t kColourLUT[] =
	{
		0x666666, 0x002a88, 0x1412a7, 0x3b00a4, 0x5c007e, 0x6e0040, 0x6c0600, 0x561d00,
		0x333500, 0x0b4800, 0x005200, 0x004f08, 0x00404d, 0x000000, 0x000000, 0x000000,
		0xadadad, 0x155fd9, 0x4240ff, 0x7527fe, 0xa01acc, 0xb71e7b, 0xb53120, 0x994e00,
		0x6b6d00, 0x388700, 0x0c9300, 0x008f32, 0x007c8d, 0x000000, 0x000000, 0x000000,
		0xfffeff, 0x64b0ff, 0x9290ff, 0xc676ff, 0xf36aff, 0xfe6ecc, 0xfe8170, 0xea9e22,
		0xbcbe00, 0x88d800, 0x5ce430, 0x45e082, 0x48cdde, 0x4f4f4f, 0x000000, 0x000000,
		0xfffeff, 0xc0dfff, 0xd3d2ff, 0xe8c8ff, 0xfbc2ff, 0xfec4ea, 0xfeccc5, 0xf7d8a5,
		0xe4e594, 0xcfef96, 0xbdf4ab, 0xb3f3cc, 0xb5ebf2, 0xb8b8b8, 0x000000, 0x000000,
	};

	lutSize = std::size(kColourLUT);
	return static_cast<const uint32_t *>(kColourLUT);
}

void NES::DMA(uint8_t page)
{
	m_pProcessor->Halt(513);
	auto page_ptr = m_pMemory->GetPagePtr(page);
	m_pPPU->doDMA(page_ptr);
}