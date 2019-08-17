#include <iostream>

#include "MOS6502Core.h"
#include "Opcodes.h"

MOS6502Core::MOS6502Core(Memory* pMemory) {
  InitOpcodeTable();

  m_pMemory = pMemory;
  m_pMemory->SetProcessor(this);
}

MOS6502Core::~MOS6502Core() {

}

void MOS6502Core::Reset() {
  /* Load reset vector */
  m_PC = (unsigned)m_pMemory->Read(RESET_VECTOR + 1) << 8u | m_pMemory->Read(RESET_VECTOR);
  m_AC = 0x00;
  m_XR = 0x00;
  m_YR = 0x00;
  m_SR = CONSTANT;
  m_SP = 0xFD;

  this->Resume();
}

uint8_t MOS6502Core::FetchOPCode() {
  return m_pMemory->Read(m_PC);
}

void MOS6502Core::ExecuteOPCode(uint8_t opcode) {
  if (!m_Running)
    return;

  Log("Opcode: %s(0x%02X), PC 0x%04X", kOPCodeNames[opcode], opcode, m_PC);
  Log("SR: 0x%02X  |  AC: 0x%02X  |  XR: 0x%02X | YR: 0x%02X | SP: 0x%02X [ %s%s%s%s%s%s%s]",
      m_SR, m_AC, m_XR, m_YR, m_SP,
      m_SR & CARRY ? "C " : "",
      m_SR & ZERO ? "Z " : "",
      m_SR & INTERRUPT ? "I ": "",
      m_SR & DECIMAL ? "D " : "",
      m_SR & BREAK ? "B " : "",
      m_SR & OVERFLOW ? "O ": "",
      m_SR & NEGATIVE ? "N ": "");
  (this->*m_OPCodes[opcode])();
  // TODO: OPCode handler should return how many cycles it took to execute.
}

uint8_t MOS6502Core::Tick() {
  ExecuteOPCode(FetchOPCode());
}

void MOS6502Core::Halt() {
  m_Running = false;
}

void MOS6502Core::Resume() {
  m_Running = true;
}
