#include "Common.hpp"

#include "AtariVCS.hpp"
#include "Cart.hpp"
#include "VCSMemory.hpp"
#include "MOS6502Core.hpp"
#include "TIACore.hpp"
#include "RIOTCore.hpp"

AtariVCS::AtariVCS()
{
	m_pCartridge = std::make_unique<Cartridge>();
	m_pMemory = std::make_shared<VCSMemory>();
	m_pProcessor = std::make_shared<MOS6502Core>(m_pMemory);
	m_pTIA = std::make_shared<TIACore>(m_pProcessor);
	m_pRIOT = std::make_shared<RIOTCore>();

	m_pMemory->SetProcessor(m_pProcessor);
	m_pMemory->SetTIA(m_pTIA);
	m_pMemory->SetRIOT(m_pRIOT);

	m_pMemory->Reset();
}

bool AtariVCS::LoadROM(const char *szFilePath)
{
	if (! m_pCartridge->LoadFromFile(szFilePath))
		return false;

	m_pMemory->LoadROM(m_pCartridge->GetROM(), m_pCartridge->GetRomSize());

	return true;
}

bool AtariVCS::LoadROM(const std::vector<uint8_t>& romBuffer)
{
	if (! m_pCartridge->LoadFromBuffer(romBuffer.data(), romBuffer.size()))
		return false;

	m_pMemory->LoadROM(m_pCartridge->GetROM(), m_pCartridge->GetRomSize());
	return true;
}

void AtariVCS::Reset()
{
	m_pProcessor->Reset();
}

void AtariVCS::RunToVBlank(std::vector<uint8_t>& framebuffer, int16_t *pSampleBuffer, int *pSampleCount, int maxSampleCount)
{
	bool frameFinished;

	do
	{
		frameFinished = m_pTIA->Tick(framebuffer);
		m_pRIOT->Tick();
		m_pProcessor->Tick();
	} while (! frameFinished);
}

void AtariVCS::KeypressEvent(keypress_event_t evt, bool pressed)
{
	switch (evt)
	{
		case KEYPRESS_UP:
			if (! pressed)
				m_pRIOT->SetSWCHA(0x10);
			else
				m_pRIOT->ClearSWCHA(0x10);
			break;

		case KEYPRESS_DOWN:
			if (! pressed)
				m_pRIOT->SetSWCHA(0x20);
			else
				m_pRIOT->ClearSWCHA(0x20);
			break;

		case KEYPRESS_LEFT:
			if (! pressed)
				m_pRIOT->SetSWCHA(0x40);
			else
				m_pRIOT->ClearSWCHA(0x40);
			break;

		case KEYPRESS_RIGHT:
			if (! pressed)
				m_pRIOT->SetSWCHA(0x80);
			else
				m_pRIOT->ClearSWCHA(0x80);

			break;

		case KEYPRESS_SPACE:
		    m_pTIA->SetTrigger(0, pressed);
			break;

		case KEYPRESS_SELECT:
			if (! pressed)
				m_pRIOT->SetSWCHB(0x02);
			else
				m_pRIOT->ClearSWCHB(0x02);
			break;

		case KEYPRESS_RESET:
			if (! pressed)
				m_pRIOT->SetSWCHB(0x01);
			else
				m_pRIOT->ClearSWCHB(0x01);
			break;
	}
}

const uint32_t* AtariVCS::GetColourLut(size_t &lutSize)
{
	static constexpr uint32_t kColourLUT[] =
	{
		0x000000, 0, 0x4a4a4a, 0, 0x6f6f6f, 0, 0x8e8e8e, 0,
		0xaaaaaa, 0, 0xc0c0c0, 0, 0xd6d6d6, 0, 0xececec, 0,
		0x484800, 0, 0x69690f, 0, 0x86861d, 0, 0xa2a22a, 0,
		0xbbbb35, 0, 0xd2d240, 0, 0xe8e84a, 0, 0xfcfc54, 0,
		0x7c2c00, 0, 0x904811, 0, 0xa26221, 0, 0xb47a30, 0,
		0xc3903d, 0, 0xd2a44a, 0, 0xdfb755, 0, 0xecc860, 0,
		0x901c00, 0, 0xa33915, 0, 0xb55328, 0, 0xc66c3a, 0,
		0xd5824a, 0, 0xe39759, 0, 0xf0aa67, 0, 0xfcbc74, 0,
		0x940000, 0, 0xa71a1a, 0, 0xb83232, 0, 0xc84848, 0,
		0xd65c5c, 0, 0xe46f6f, 0, 0xf08080, 0, 0xfc9090, 0,
		0x840064, 0, 0x97197a, 0, 0xa8308f, 0, 0xb846a2, 0,
		0xc659b3, 0, 0xd46cc3, 0, 0xe07cd2, 0, 0xec8ce0, 0,
		0x500084, 0, 0x68199a, 0, 0x7d30ad, 0, 0x9246c0, 0,
		0xa459d0, 0, 0xb56ce0, 0, 0xc57cee, 0, 0xd48cfc, 0,
		0x140090, 0, 0x331aa3, 0, 0x4e32b5, 0, 0x6848c6, 0,
		0x7f5cd5, 0, 0x956fe3, 0, 0xa980f0, 0, 0xbc90fc, 0,
		0x000094, 0, 0x181aa7, 0, 0x2d32b8, 0, 0x4248c8, 0,
		0x545cd6, 0, 0x656fe4, 0, 0x7580f0, 0, 0x8490fc, 0,
		0x001c88, 0, 0x183b9d, 0, 0x2d57b0, 0, 0x4272c2, 0,
		0x548ad2, 0, 0x65a0e1, 0, 0x75b5ef, 0, 0x84c8fc, 0,
		0x003064, 0, 0x185080, 0, 0x2d6d98, 0, 0x4288b0, 0,
		0x54a0c5, 0, 0x65b7d9, 0, 0x75cceb, 0, 0x84e0fc, 0,
		0x004030, 0, 0x18624e, 0, 0x2d8169, 0, 0x429e82, 0,
		0x54b899, 0, 0x65d1ae, 0, 0x75e7c2, 0, 0x84fcd4, 0,
		0x004400, 0, 0x1a661a, 0, 0x328432, 0, 0x48a048, 0,
		0x5cba5c, 0, 0x6fd26f, 0, 0x80e880, 0, 0x90fc90, 0,
		0x143c00, 0, 0x355f18, 0, 0x527e2d, 0, 0x6e9c42, 0,
		0x87b754, 0, 0x9ed065, 0, 0xb4e775, 0, 0xc8fc84, 0,
		0x303800, 0, 0x505916, 0, 0x6d762b, 0, 0x88923e, 0,
		0xa0ab4f, 0, 0xb7c25f, 0, 0xccd86e, 0, 0xe0ec7c, 0,
		0x482c00, 0, 0x694d14, 0, 0x866a26, 0, 0xa28638, 0,
		0xbb9f47, 0, 0xd2b656, 0, 0xe8cc63, 0, 0xfce070, 0
	};

	lutSize = std::size(kColourLUT);
	return static_cast<const uint32_t*>(kColourLUT);
}

const EmulatorCore::framebufferInfo AtariVCS::GetFramebufferInfo()
{
	static constexpr EmulatorCore::framebufferInfo info =
	{
		.width 	= 160,
		.height = 200,
		.scaleX = 3,
		.scaleY = 2,
		.framerate = 120
	};

	return info;
}
