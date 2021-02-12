#pragma once

#include "EmulatorCore.hpp"
#include "SDL.h"

#include <memory>
#include <cstdint>

class Emulator
{
public:
	Emulator();

	void RunToVBlank();
	bool LoadRom(const char* szFilePath);

	bool Running() const;
	void Stop();

	bool Draw(SDL_Renderer* renderer, SDL_Texture* texture);
	void UpdateTexture(SDL_Texture* texture);
	void InitSDL(SDL_Renderer** renderer, SDL_Texture** texture);

	void KeypressEvent(keypress_event_t evt, bool pressed);

private:
	bool m_running = true;
	std::unique_ptr<EmulatorCore> m_emulatorCore;
	const uint32_t* m_ColourLUT = nullptr;
	size_t m_lutSize = 0;

	std::vector<uint8_t> m_framebuffer;
	EmulatorCore::framebufferInfo m_framebufferInfo;
};