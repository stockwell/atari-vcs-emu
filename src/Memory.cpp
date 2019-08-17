#include <iostream>
#include <cstring>

#include "Memory.h"
#include "TIACore.h"

#define TIA_START_ADDR  0x00
#define TIA_END_ADDR    0x7F

#define RAM_START_ADDR  0x80
#define RAM_END_ADDR    0xFF

#define STACK_START_ADDR 0x100
#define STACK_END_ADDR   0x1FF

#define RIOT_START_ADDR 0x200
#define RIOT_END_ADDR   0x2FF

#define ROM_START_ADDR  0x1000
#define ROM_END_ADDR    0x1FFF

Memory::Memory() {
  m_pMap = new uint8_t[65536];
}

Memory::~Memory() {
  SafeDeleteArray(m_pMap)
}

void Memory::SetProcessor(MOS6502Core *pProcessor) {
  m_pProcessor = pProcessor;
}

void Memory::SetTIA(TIACore *pTIA) {
  m_pTIA = pTIA;
}

void Memory::Reset() {
  for (int i = 0; i < 65536; i++) {
    m_pMap[i] = 0x00;
  }
}

void Memory::LoadROM(const uint8_t *pROM, uint16_t romSize) {
  /* For a 2K ROM cart A11 isn't connected - effectively 0x1000 - 0x17FF is mirrored from 0x1800 - 0x1FFF */
  for (int i = ROM_START_ADDR, j = 0; i < ROM_END_ADDR; i++, j++) {
    m_pMap[i] = pROM[j % romSize];
  }
}

void Memory::Load(uint16_t address, uint8_t *bytes, size_t numBytes) {
  /* YOLO */
  memcpy(m_pMap + (address & 0x1FFFu), bytes, numBytes);
}

void Memory::Load(uint16_t address, uint8_t byte) {
  /* YOLO */
  m_pMap[address & 0x1FFFu] = byte;
}

uint8_t Memory::Read(uint16_t address) {

  /* MOS 6507 doesn't have address lines 13-15 connected. Only the first 13 bits of the address actually matter */
  uint16_t actualAddress = address & 0x1FFFu;

  if (actualAddress >= TIA_START_ADDR && actualAddress <= TIA_END_ADDR) {
    m_pTIA->Read(actualAddress);
    return m_pMap[actualAddress];
  } else if (actualAddress >= RAM_START_ADDR && actualAddress <= RAM_END_ADDR) {
    return m_pMap[actualAddress];
  } else if (actualAddress >= STACK_START_ADDR && actualAddress <= STACK_END_ADDR) {
    return m_pMap[actualAddress];
  } else if (actualAddress >= RIOT_START_ADDR && actualAddress <= RIOT_END_ADDR) {
    return m_pMap[actualAddress];
  } else if (actualAddress >= ROM_START_ADDR && actualAddress <= ROM_END_ADDR) {
    return m_pMap[actualAddress];
  }

  return 0xFF;
}

void Memory::Write(uint16_t address, uint8_t value) {
  /* MOS 6507 doesn't have address lines 13-15 connected. Only the first 13 bits of the address actually matter */
  uint16_t actualAddress = address & 0x1FFFu;

  if (actualAddress >= TIA_START_ADDR && actualAddress <= TIA_END_ADDR) {
    /* For convenience reads and write requests will be buffered in m_pMap, consider removing if this isn't useful */
    m_pMap[actualAddress] = value; /* Note: Buffered value may not be valid */
    m_pTIA->Write(actualAddress, value);
  } else if (actualAddress >= RAM_START_ADDR && actualAddress <= RAM_END_ADDR) {
    m_pMap[actualAddress] = value;
  } else if (actualAddress >= STACK_START_ADDR && actualAddress <= STACK_END_ADDR) {
    m_pMap[actualAddress] = value;
  } else if (actualAddress >= RIOT_START_ADDR && actualAddress <= RIOT_END_ADDR) {
    m_pMap[actualAddress] = value;
    /* Dispatch to RIOT core */
  } else {
    std::cout << "Invalid write address! Addr = " << std::hex << address << std::endl;
  }
}
