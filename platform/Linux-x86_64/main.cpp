#include <iostream>
#include <mutex>

#include "SDL.h"

#include "main.h"

int frame;

SDL_Renderer *renderer;
SDL_Texture *EmuTexture;

void UpdateTexture(SDL_Texture *texture, const uint8_t *framebuffer) {
  Uint32 src = 0;
  Uint32 *dst;
  int row, col;
  void *pixels;
  int pitch;

  if (SDL_LockTexture(texture, NULL, &pixels, &pitch) < 0) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't lock texture: %s\n", SDL_GetError());
  }

  for (row = 0; row < ATARI_2600_FB_H; ++row) {
    dst = (Uint32*)((Uint8*)pixels + row * pitch);
    for (col = 0; col < ATARI_2600_FB_W; ++col) {
      *dst++ = (0xFF000000|(ColourLUT[framebuffer[src++] & 0xFEu]));
    }
  }
  SDL_UnlockTexture(texture);
}

void SDL_Init() {
  SDL_Window *window;
  SDL_RWops *handle;

  /* Enable standard application logging */
  SDL_LogSetPriority(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_VERBOSE);

  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s\n", SDL_GetError());
    return;
  }

  /* Create the window and renderer */
  window = SDL_CreateWindow("Atari 2600 Emu",
                            SDL_WINDOWPOS_UNDEFINED,
                            SDL_WINDOWPOS_UNDEFINED,
                            EMU_WINDOW_W, EMU_WINDOW_H,
                            SDL_WINDOW_RESIZABLE);
  if (!window) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't set create window: %s\n", SDL_GetError());
  }

  renderer = SDL_CreateRenderer(window, -1, 0);
  if (!renderer) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't set create renderer: %s\n", SDL_GetError());
  }

  EmuTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, ATARI_2600_FB_W, ATARI_2600_FB_H);
  if (!EmuTexture) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't set create texture: %s\n", SDL_GetError());
  }
}

Emulator::Emulator() {
  m_pAtariVCS = new AtariVCS();
}

Emulator::~Emulator() {
  SafeDelete(m_pAtariVCS)
}

void Emulator::RunToVBlank(uint8_t* pFrameBuffer) {
  m_pAtariVCS->RunToVBlank(pFrameBuffer, nullptr, nullptr);
}

bool Emulator::LoadRom(const char *szFilePath) {
  if (!m_pAtariVCS->LoadROM(szFilePath)) return false;
  m_pAtariVCS->Reset();
  return true;
}

bool Emulator::Running() {
  return running;
}

void Emulator::Stop() {
  running = false;
}

void Emulator::KeypressEvent(keypress_event_t evt, bool pressed) {
  m_pAtariVCS->KeypressEvent(evt, pressed);
}

bool Emulator::Draw(uint8_t* pFramebuffer) {
  SDL_Event event;

  while (SDL_PollEvent(&event)) {
    switch (event.type) {
      case SDL_KEYUP:
      case SDL_KEYDOWN: {
        switch (event.key.keysym.sym) {
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

          default:
            break;
        }
      } break;

      case SDL_QUIT:
        return false;
    }
  }

  UpdateTexture(EmuTexture, pFramebuffer);

  SDL_RenderClear(renderer);
  SDL_RenderCopy(renderer, EmuTexture, NULL, NULL);
  SDL_RenderPresent(renderer);

  return true;
}

int main() {
  auto emulator = new Emulator();

  if (!emulator->LoadRom("kernel_21.bin")) {
    exit(1);
  }

  // NTSC Resolution
  auto *framebuffer = new uint8_t[160*192];

#ifndef DISABLE_RENDERER
  SDL_Init();
#endif

  /* Loop, waiting for QUIT or the escape key */
  do {
    emulator->RunToVBlank(framebuffer);

#ifndef DISABLE_RENDERER
    if (!emulator->Draw(framebuffer)) {
      emulator->Stop();
    }
#endif

  } while (emulator->Running());

#ifndef DISABLE_RENDERER
  SDL_DestroyRenderer(renderer);
#endif

  SafeDelete(emulator)
  SafeDeleteArray(framebuffer)

  return 0;
}
