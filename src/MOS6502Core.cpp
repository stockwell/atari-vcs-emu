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
  PC = (unsigned)m_pMemory->Read(0xfffd) << 8u | m_pMemory->Read(0xfffc);
  AC = 0x00;
  XR = 0x00;
  YR = 0x00;
  SR = 0x20;
  SP = 0xFF;
}

uint8_t MOS6502Core::FetchOPCode() {
  return m_pMemory->Read(PC);
}

void MOS6502Core::ExecuteOPCode(uint8_t opcode) {
  printf("Opcode: %s(0x%02X), PC 0x%04X\n", kOPCodeNames[opcode], opcode, PC);
  (this->*m_OPCodes[opcode])();

}

uint8_t MOS6502Core::Tick() {
  ExecuteOPCode(FetchOPCode());
}