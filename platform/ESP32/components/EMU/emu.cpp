#include <iostream>
#include <mutex>
#include <vector>

#include "kernel.h"
#include "AtariVCS.h"

//#define DISABLE_RENDERER

#define ATARI_2600_W 160
#define ATARI_2600_H 192

class Emulator
{
public:
  Emulator() = default;
  ~Emulator();
  void Init();
  void RunToVBlank();
  void LoadRom(std::vector<uint8_t> &buffer);
  bool Running();
  void Stop();

public:
  uint32_t m_Framebuffer[160*192];

private:
  bool running = true;
  AtariVCS *m_pAtariVCS = nullptr;
  std::mutex m;
};

bool draw(uint32_t* framebuffer) {
  return true;
}

void emu() {
  auto emulator = new Emulator();
  emulator->Init();

  std::vector<uint8_t> rom(kernel_01_bin, kernel_01_bin + sizeof kernel_01_bin / sizeof kernel_01_bin[0]);
  emulator->LoadRom(rom);

  /* Loop forever */

  do {
    emulator->RunToVBlank();
    //emulator->Stop();

  } while (emulator->Running());

  SafeDelete(emulator)
}

extern "C" void emulator_main(void) {
  emu();
}

Emulator::~Emulator() {
  SafeDelete(m_pAtariVCS)
}

void Emulator::Init() {
  m_pAtariVCS = new AtariVCS();
  m_pAtariVCS->Init();
}

void Emulator::RunToVBlank() {
  m.lock();
  m_pAtariVCS->RunToVBlank(m_Framebuffer, nullptr, nullptr);
  m.unlock();
}

void Emulator::LoadRom(std::vector<uint8_t> &buffer) {
  m.lock();
  m_pAtariVCS->LoadROM(&buffer);
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

