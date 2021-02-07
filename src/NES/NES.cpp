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

    m_pMemory->SetProcessor(m_pProcessor);
    m_pMemory->Reset();
}

bool NES::LoadROM(const char *szFilePath)
{
    if (! m_pCartridge->LoadFromFile(szFilePath))
        return false;

    m_pMemory->LoadROM(m_pCartridge->GetROM(), m_pCartridge->GetRomSize());

    return true;
}

bool NES::LoadROM(const std::vector<uint8_t>& romBuffer)
{
    if (! m_pCartridge->LoadFromBuffer(romBuffer.data(), romBuffer.size()))
        return false;

    m_pMemory->LoadROM(m_pCartridge->GetROM(), m_pCartridge->GetRomSize());
    return true;
}

void NES::Reset()
{
    m_pProcessor->Reset();
}

void NES::RunToVBlank(std::vector<uint8_t>& framebuffer, int16_t *pSampleBuffer, int *pSampleCount)
{
    while (true)
    {
        m_pProcessor->Tick();
    }
}

void NES::KeypressEvent(keypress_event_t evt, bool pressed)
{
    switch (evt)
    {
        case KEYPRESS_UP:
            break;

        case KEYPRESS_DOWN:
            break;

        case KEYPRESS_LEFT:
            break;

        case KEYPRESS_RIGHT:
            break;

        case KEYPRESS_SPACE:
            break;

        case KEYPRESS_SELECT:
            break;

        case KEYPRESS_RESET:
            break;
    }
}
