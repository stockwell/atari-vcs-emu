#pragma once

#include "VCS/AtariVCS.hpp"
#include "SDL.h"

#include <memory>

class Emulator
{
public:
	Emulator();

	void RunToVBlank(std::vector<uint8_t>& framebuffer);
	bool LoadRom(const char* szFilePath);
	bool Running() const;
	void Stop();
	bool Draw(std::vector<uint8_t>& framebuffer, SDL_Renderer* renderer, SDL_Texture* texture);
	void KeypressEvent(keypress_event_t evt, bool pressed);

private:
	bool m_running = true;
	std::unique_ptr<AtariVCS> m_pAtariVCS;
};