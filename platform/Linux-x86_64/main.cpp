#include <iostream>
#include <mutex>

#include "SDL.h"

#include "main.h"
#include "AtariVCS.h"

class Emulator
{
public:
  Emulator() = default;
  ~Emulator();
  void Init();
  void RunToVBlank(uint8_t* pFrameBuffer);
  void LoadRom(const char* szFilePath);
  bool Running();
  void Stop();
  bool Draw(uint8_t* pFramebuffer);
private:
  bool running = true;
  AtariVCS *m_pAtariVCS = nullptr;
  std::mutex m;
};

#define ATARI_2600_FB_W 160
#define ATARI_2600_FB_H 192

#define EMU_WINDOW_W  900
#define EMU_WINDOW_H  600

SDL_Renderer *renderer;
int frame;
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

int main() {
  auto emulator = new Emulator();
  emulator->Init();
  emulator->LoadRom("kernel_01.bin");

  // NTSC Resolution
  auto *framebuffer = new uint8_t[160*192];

#ifndef DISABLE_RENDERER
  SDL_Init();
#endif

  /* Loop, waiting for QUIT or the escape key */
  uint8_t frames = 2;
  do {
    emulator->RunToVBlank(framebuffer);
    if (!--frames) {
      //emulator->Stop();
    }

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

Emulator::~Emulator() {
  SafeDelete(m_pAtariVCS)
}

void Emulator::Init()
{
  m_pAtariVCS = new AtariVCS();
  m_pAtariVCS->Init();
}

void Emulator::RunToVBlank(uint8_t* pFrameBuffer) {
  m.lock();
  m_pAtariVCS->RunToVBlank(pFrameBuffer, nullptr, nullptr);
  m.unlock();
}

void Emulator::LoadRom(const char *szFilePath) {
  m.lock();
  m_pAtariVCS->LoadROM(szFilePath);
  m_pAtariVCS->Reset();
  m.unlock();
}

bool Emulator::Running() {
  return running;
}

void Emulator::Stop() {
  m.lock();
  running = false;
  m.unlock();
}

bool Emulator::Draw(uint8_t* pFramebuffer) {
  SDL_Event event;

  while (SDL_PollEvent(&event)) {
    switch (event.type) {
      case SDL_KEYDOWN:
        if (event.key.keysym.sym == SDLK_ESCAPE) {
          return false;
        }
        break;
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
