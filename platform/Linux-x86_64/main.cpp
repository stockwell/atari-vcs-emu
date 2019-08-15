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
  emulator->LoadRom("adventure.bin");

  // NTSC Resolution
  auto *framebuffer = new CRGBA[160*192];

  do {
    emulator->RunToVBlank(framebuffer);
    emulator->Stop();

  } while (emulator->Running());

  SafeDelete(emulator);

  return 0;
}

Emulator::Emulator() {

}

Emulator::~Emulator() {
  SafeDelete(m_pAtariVCS)
}

void Emulator::Init()
{
  m_pAtariVCS = new AtariVCS();
  m_pAtariVCS->Init();
}

void Emulator::RunToVBlank(CRGBA* pFrameBuffer) {
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
