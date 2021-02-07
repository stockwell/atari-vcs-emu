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

void AtariVCS::RunToVBlank(std::vector<uint8_t>& framebuffer, int16_t *pSampleBuffer, int *pSampleCount)
{
	while (true)
	{

		if (m_pTIA->Tick(framebuffer))
		{
			//memset(framebuffer.data(), 0x00, framebuffer.size());
			break;
		}
        m_pProcessor->Tick();
        m_pRIOT->Tick();
		//break;
	}
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
