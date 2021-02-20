#include <chrono>
#include <unistd.h>
#include "main.hpp"
#include "VCS/AtariVCS.hpp"
#include "NES/NES.hpp"

void Emulator::UpdateTexture(SDL_Texture* texture)
{
	uint32_t src = 0;
	uint32_t *dst;
	int row, col;
	void* pixels;
	int pitch;

	if (SDL_LockTexture(texture, nullptr, &pixels, &pitch) < 0)
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't lock texture: %s\n", SDL_GetError());

	for (row = 0; row < m_framebufferInfo.height; ++row)
	{
		dst = (uint32_t*) ((uint8_t*) pixels + row * pitch);
		for (col = 0; col < m_framebufferInfo.width; ++col)
			*dst++ = (0xFF000000 | (m_ColourLUT[m_framebuffer[src++]]));
	}
	SDL_UnlockTexture(texture);
}

void Emulator::InitSDL(SDL_Renderer** renderer, SDL_Texture** texture)
{
	SDL_Window* window;

	/* Enable standard application logging */
	SDL_LogSetPriority(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_VERBOSE);

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s\n", SDL_GetError());
		return;
	}

	/* Create the window and renderer */
	window = SDL_CreateWindow("Emulator",
							  SDL_WINDOWPOS_UNDEFINED,
							  SDL_WINDOWPOS_UNDEFINED,
							  m_framebufferInfo.width * m_framebufferInfo.scaleX,
							  m_framebufferInfo.height * m_framebufferInfo.scaleY,
							  SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_RESIZABLE);
	if (! window)
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't set create window: %s\n", SDL_GetError());

	*renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (! *renderer)
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't set create renderer: %s\n", SDL_GetError());

	*texture = SDL_CreateTexture(*renderer,
								   SDL_PIXELFORMAT_ARGB8888,
								   SDL_TEXTUREACCESS_STREAMING,
								   m_framebufferInfo.width,
								   m_framebufferInfo.height);
	if (! *texture)
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't set create texture: %s\n", SDL_GetError());
}

Emulator::Emulator()
{
#ifdef NES_EMULATOR
	m_emulatorCore = std::make_unique<NES>();
#elif VCS_EMULATOR
	m_emulatorCore = std::make_unique<AtariVCS>();
#endif

	m_framebufferInfo = m_emulatorCore->GetFramebufferInfo();
	m_framebuffer.resize(m_framebufferInfo.height * m_framebufferInfo.width);

	m_ColourLUT = m_emulatorCore->GetColourLut(m_lutSize);
}

void Emulator::RunToVBlank()
{
	m_emulatorCore->RunToVBlank(m_framebuffer, nullptr, nullptr);
}

bool Emulator::LoadRom(const char *szFilePath)
{
	if (! m_emulatorCore->LoadROM(szFilePath))
		return false;

	m_emulatorCore->Reset();
	return true;
}

bool Emulator::Running() const
{
	return m_running;
}

void Emulator::Stop()
{
	m_running = false;
}

void Emulator::KeypressEvent(keypress_event_t evt, bool pressed)
{
	m_emulatorCore->KeypressEvent(evt, pressed);
}

bool Emulator::Draw(SDL_Renderer* renderer, SDL_Texture* texture)
{
	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			case SDL_KEYUP:
			case SDL_KEYDOWN:
			{
				switch (event.key.keysym.sym)
				{
					case SDLK_ESCAPE:
						return false;

					case SDLK_RIGHT:
						KeypressEvent(KEYPRESS_RIGHT, event.type == SDL_KEYDOWN);
						break;

					case SDLK_LEFT:
						KeypressEvent(KEYPRESS_LEFT, event.type == SDL_KEYDOWN);
						break;

					case SDLK_UP:
						KeypressEvent(KEYPRESS_UP, event.type == SDL_KEYDOWN);
						break;

					case SDLK_DOWN:
						KeypressEvent(KEYPRESS_DOWN, event.type == SDL_KEYDOWN);
						break;

					case SDLK_SPACE:
						KeypressEvent(KEYPRESS_SPACE, event.type == SDL_KEYDOWN);
						break;

					case SDLK_F10:
						KeypressEvent(KEYPRESS_START, event.type == SDL_KEYDOWN);
						break;

					case SDLK_F11:
						KeypressEvent(KEYPRESS_SELECT, event.type == SDL_KEYDOWN);
						break;

					case SDLK_F12:
						KeypressEvent(KEYPRESS_RESET, event.type == SDL_KEYDOWN);
						break;

					case SDLK_z:
						KeypressEvent(KEYPRESS_A, event.type == SDL_KEYDOWN);
						break;

					case SDLK_x:
						KeypressEvent(KEYPRESS_B, event.type == SDL_KEYDOWN);
						break;

					default:break;
				}
				break;
			}
			case SDL_QUIT:
				return false;
		}
	}

	UpdateTexture(texture);
	
	SDL_RenderCopy(renderer, texture, nullptr, nullptr);
	SDL_RenderPresent(renderer);

	return true;
}

int main()
{
	SDL_Renderer*	renderer;
	SDL_Texture*	texture;

	setbuf(stdout, nullptr);

	auto emulator = std::make_unique<Emulator>();

#ifdef NES_EMULATOR
	if (! emulator->LoadRom("mario.nes"))
		exit(1);
#elif VCS_EMULATOR
	if (! emulator->LoadRom("pitfall.bin"))
		exit(1);
#endif

#ifndef DISABLE_RENDERER
	emulator->InitSDL(&renderer, &texture);
#endif

	/* Loop, waiting for QUIT or the escape key */
	do
	{
		auto start = std::chrono::steady_clock::now();
		emulator->RunToVBlank();

#ifndef DISABLE_RENDERER
		if (! emulator->Draw(renderer, texture))
			emulator->Stop();
#endif

		std::chrono::duration<double> elapsed_seconds = std::chrono::steady_clock::now() - start;

		auto delay = static_cast<int32_t>(((1.0f/static_cast<float>(emulator->GetFramerate())) - elapsed_seconds.count()) * 1000000);

		if (delay > 0)
			usleep(delay);

	} while (emulator->Running());

#ifndef DISABLE_RENDERER
	SDL_DestroyRenderer(renderer);
#endif

	return 0;
}

int WinMain()
{
	main();
}
