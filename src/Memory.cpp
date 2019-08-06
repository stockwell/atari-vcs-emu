#include <iostream>

#include "Memory.h"

#define TIA_START_ADDR  0x00
#define TIA_END_ADDR    0x7F

#define RAM_START_ADDR  0x80
#define RAM_END_ADDR    0xFF

#define RIOT_START_ADDR 0x200
#define RIOT_END_ADDR   0x2FF

#define ROM_START_ADDR  0x1000
#define ROM_END_ADDR    0x1FFF

Memory::Memory() {

}

Memory::~Memory() {
  SafeDeleteArray(m_pMap)
}

void Memory::SetProcessor(MOS6502Core *pProcessor) {
  m_pProcessor = pProcessor;
}

void Memory::Init() {
  m_pMap = new uint8_t[65536];
}

void Memory::Reset() {
  for (int i = 0; i < 65536; i++) {
    m_pMap[i] = 0x00;
  }
}

void Memory::LoadROM(uint8_t *pROM) {
  /* Careful - this assumes a 4K ROM */
  for (int i = ROM_START_ADDR, j = 0; i < ROM_END_ADDR; i++, j++) {
    m_pMap[i] = pROM[j];
  }
}

uint8_t Memory::Read(uint16_t address) {

  /* MOS 6507 doesn't have address lines 13-15 connected. Only the first 13 bits of the address actually matter */
  uint16_t actualAddress = address & 0x1FFF;

  if (actualAddress >= ROM_START_ADDR && actualAddress <= ROM_END_ADDR) {
    uint8_t val = m_pMap[actualAddress];
    return val;
  }

  return 0xFF;
}