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
  m_PC = (unsigned)m_pMemory->Read(RESET_VECTOR + 1) << 8u | m_pMemory->Read(RESET_VECTOR);
  m_AC = 0x00;
  m_XR = 0x00;
  m_YR = 0x00;
  m_SR = CONSTANT;
  m_SP = 0xFD;
}

uint8_t MOS6502Core::FetchOPCode() {
  return m_pMemory->Read(m_PC);
}

void MOS6502Core::ExecuteOPCode(uint8_t opcode) {
  Log("Opcode: %s(0x%02X), PC 0x%04X", kOPCodeNames[opcode], opcode, m_PC);
  (this->*m_OPCodes[opcode])();
  Log("SR: 0x%02X  |  AC: 0x%02X  |  XR: 0x%02X | YR: 0x%02X | SP: 0x%02X", m_SR, m_AC, m_XR, m_YR, m_SP);
  Log("%s%s%s%s%s%s%s",
      m_SR & CARRY ? "C " : "",
      m_SR & ZERO ? "Z " : "",
      m_SR & INTERRUPT ? "I ": "",
      m_SR & DECIMAL ? "D " : "",
      m_SR & BREAK ? "B " : "",
      m_SR & OVERFLOW ? "O ": "",
      m_SR & NEGATIVE ? "N ": ""
      );
}

uint8_t MOS6502Core::Tick() {
  ExecuteOPCode(FetchOPCode());
}
