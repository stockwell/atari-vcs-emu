#pragma once

#include <memory>
#include <vector>

#include "Common.hpp"

#include "Cart.hpp"

class MOS6502Core;
class VCSMemory;
class TIACore;
class RIOTCore;

typedef enum
{
	KEYPRESS_UP,
	KEYPRESS_DOWN,
	KEYPRESS_LEFT,
	KEYPRESS_RIGHT,
	KEYPRESS_SPACE,
	KEYPRESS_RESET,
	KEYPRESS_SELECT
} keypress_event_t;

class AtariVCS
{
public:
	AtariVCS();

	bool LoadROM(const char *szFilePath);
	bool LoadROM(const std::vector<uint8_t>& romBuffer);

	void RunToVBlank(std::vector<uint8_t>& framebuffer, int16_t *pSampleBuffer, int *pSampleCount);

	void Reset();

	void KeypressEvent(keypress_event_t evt, bool pressed);

private:
	std::unique_ptr<Cartridge>		m_pCartridge = nullptr;
	std::shared_ptr<MOS6502Core>	m_pProcessor = nullptr;
	std::shared_ptr<VCSMemory>		m_pMemory = nullptr;
	std::shared_ptr<TIACore>		m_pTIA = nullptr;
	std::shared_ptr<RIOTCore>		m_pRIOT = nullptr;
};
