#include <iostream>
#include <mutex>
#include <vector>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "emu.h"
#include "display.h"
#include "kernel.h"
#include "AtariVCS.h"

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
  void Draw();

public:
  uint8_t m_Framebuffer[160*192];

private:
  bool running = true;
  AtariVCS *m_pAtariVCS = nullptr;
  std::mutex m;
};

void emu() {
  auto emulator = new Emulator();
  emulator->Init();

  std::vector<uint8_t> rom(kernel_01_bin, kernel_01_bin + sizeof kernel_01_bin / sizeof kernel_01_bin[0]);
  emulator->LoadRom(rom);

  /* Loop forever */
  do {
    uint32_t start = (uint32_t)esp_timer_get_time();
    emulator->RunToVBlank();
    emulator->Draw();
    //vTaskDelay(1);
    uint32_t time = (uint32_t)esp_timer_get_time();
    std::cout << (time - start) << std::endl;
  } while (emulator->Running());

  SafeDelete(emulator)
}

extern "C" void emulator_main(void *arg) {
  printf("Starting Atari VCS Emu..\n");
  emu();
  printf("Emulator shutting down..\n");
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

void Emulator::Draw() {
  write_frame_atari2600_async(m_Framebuffer, ColourLUT_16b, false);
}