#include <iostream>
#include <mutex>

#include "AtariVCS.h"

class Emulator
{
public:
  Emulator();
  ~Emulator();
  void Init();
  void RunToVBlank(CRGBA* pFrameBuffer);
  void LoadRom(const char* szFilePath);
  bool Running();
  void Stop();

private:
  bool running = true;
  AtariVCS *m_pAtariVCS = nullptr;
  std::mutex m;
};

int main() {
  auto emulator = new Emulator();
  emulator->Init();
  emulator->LoadRom("pacman.bin");

  // NTSC Resolution
  auto *framebuffer = new CRGBA[160*192];

  do {
    emulator->RunToVBlank(framebuffer);
    emulator->Stop();

  } while (emulator->Running());

  delete(emulator);

  return 0;
}

Emulator::Emulator() {

}

Emulator::~Emulator() {
  delete(m_pAtariVCS);
}

void Emulator::Init()
{
  m_pAtariVCS = new AtariVCS();
  m_pAtariVCS->Init();
}

void Emulator::RunToVBlank(CRGBA* pFrameBuffer) {
  m.lock();

  m.unlock();
}

void Emulator::LoadRom(const char *szFilePath) {
  m.lock();
  m_pAtariVCS->LoadROM(szFilePath);
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