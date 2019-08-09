#include <iostream>

#include "MOS6502Core.h"
#include "Opcodes.h"

MOS6502Core::MOS6502Core() {
  m_pMemory = nullptr;

  InitOpcodeTable();
}

MOS6502Core::~MOS6502Core() {

}

void MOS6502Core::Init(Memory* pMemory) {
  m_pMemory = pMemory;
  pMemory->SetProcessor(this);
}

void MOS6502Core::Reset() {
  /* Load reset vector */
  m_PC = (unsigned)m_pMemory->Read(0xfffd) << 8u | m_pMemory->Read(0xfffc);
  m_AC = 0x00;
  m_XR = 0x00;
  m_YR = 0x00;
  m_SR = 0x20;
  m_SP = 0xFF;
}

uint8_t MOS6502Core::FetchOPCode() {
  return m_pMemory->Read(m_PC);
}

void MOS6502Core::ExecuteOPCode(uint8_t opcode) {
  uint16_t PC = m_PC;
  (this->*m_OPCodes[opcode])();
  printf("Opcode: %s(0x%02X), PC 0x%04X\n", kOPCodeNames[opcode], opcode, PC);
  printf("SR: 0x%02X  |  AC: 0x%02X  |  XR: 0x%02X | YR: 0x%02X | SR: 0x%02X | SP: 0x%02X\n", m_SR, m_AC, m_XR, m_YR, m_SR, m_SP);
}

uint8_t MOS6502Core::Tick() {
  ExecuteOPCode(FetchOPCode());
}
