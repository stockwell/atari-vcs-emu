#include <iostream>

#include "Common.h"
#include "MOS6502Core.h"
#include "Opcodes.h"

void MOS6502Core::InitOpcodeTable() {
  for (auto & m_OPCode : m_OPCodes) {
    m_OPCode = &MOS6502Core::OPCodeInvalid;
  }

  m_OPCodes[0x00] = &MOS6502Core::OPCode0x00;
  m_OPCodes[0x01] = &MOS6502Core::OPCode0x01;
  m_OPCodes[0x05] = &MOS6502Core::OPCode0x05;
  m_OPCodes[0x06] = &MOS6502Core::OPCode0x06;
  m_OPCodes[0x08] = &MOS6502Core::OPCode0x08;
  m_OPCodes[0x09] = &MOS6502Core::OPCode0x09;
  m_OPCodes[0x0A] = &MOS6502Core::OPCode0x0A;
  m_OPCodes[0x0D] = &MOS6502Core::OPCode0x0D;
  m_OPCodes[0x0E] = &MOS6502Core::OPCode0x0E;

  m_OPCodes[0x10] = &MOS6502Core::OPCode0x10;
  m_OPCodes[0x18] = &MOS6502Core::OPCode0x18;

  m_OPCodes[0x20] = &MOS6502Core::OPCode0x20;

  m_OPCodes[0x30] = &MOS6502Core::OPCode0x30;
  m_OPCodes[0x38] = &MOS6502Core::OPCode0x38;

  m_OPCodes[0x49] = &MOS6502Core::OPCode0x49;
  m_OPCodes[0x4C] = &MOS6502Core::OPCode0x4C;

  m_OPCodes[0x60] = &MOS6502Core::OPCode0x60;

  m_OPCodes[0x78] = &MOS6502Core::OPCode0x78;

  m_OPCodes[0x84] = &MOS6502Core::OPCode0x84;
  m_OPCodes[0x88] = &MOS6502Core::OPCode0x88;

  m_OPCodes[0x95] = &MOS6502Core::OPCode0x95;
  m_OPCodes[0x9A] = &MOS6502Core::OPCode0x9A;

  m_OPCodes[0xA0] = &MOS6502Core::OPCode0xA0;
  m_OPCodes[0xA2] = &MOS6502Core::OPCode0xA2;
  m_OPCodes[0xA9] = &MOS6502Core::OPCode0xA9;

  m_OPCodes[0xB0] = &MOS6502Core::OPCode0xB0;

  m_OPCodes[0xCA] = &MOS6502Core::OPCode0xCA;
  m_OPCodes[0xC8] = &MOS6502Core::OPCode0xC8;

  m_OPCodes[0xD8] = &MOS6502Core::OPCode0xD8;

  m_OPCodes[0xE9] = &MOS6502Core::OPCode0xE9;
}

void MOS6502Core::OPCodeInvalid() {
  std::cout << "INVALID OPCODE!!" << std::endl;
}

void MOS6502Core::OPCode0x00() {

}

void MOS6502Core::OPCode0x01() {

}

void MOS6502Core::OPCode0x05() {

}

void MOS6502Core::OPCode0x06() {

}

void MOS6502Core::OPCode0x08() {

}

void MOS6502Core::OPCode0x09() {

}

/* ASL A */
void MOS6502Core::OPCode0x0A() {
  m_AC & 0x80 ? m_SR |= CARRY : m_SR &= ~CARRY;
  m_AC <<= 1;
  ++m_PC;

  m_AC & 0x80 ? m_SR |= NEGATIVE : m_SR &=~NEGATIVE;
  m_AC ? m_SR &= ~ZERO : m_SR |= ZERO;
}


void MOS6502Core::OPCode0x0D() {

}

/* ASL ABS */
void MOS6502Core::OPCode0x0E() {
  uint16_t address = m_pMemory->Read(m_PC + 1) | (m_pMemory->Read(m_PC + 2)) << 8u;
  uint8_t val = m_pMemory->Read(address);
  val & 0x80 ? m_SR |= CARRY : m_SR &= ~CARRY;

  val <<= 1;
  m_pMemory->Write(address, val);
  m_PC += 3;

  val & 0x80 ? m_SR |= NEGATIVE : m_SR &=~NEGATIVE;
  val ? m_SR &= ~ZERO : m_SR |= ZERO;
}

/* BPL relative */
void MOS6502Core::OPCode0x10() {
  if ((m_SR & NEGATIVE) == 0) {
    m_PC += (int8_t)m_pMemory->Read(++m_PC);
    ++m_PC;
  } else {
    m_PC += 2;
  }
}

void MOS6502Core::OPCode0x11() {

}

void MOS6502Core::OPCode0x15() {

}

void MOS6502Core::OPCode0x16() {

}

/* CLC */
void MOS6502Core::OPCode0x18() {
  m_SR &= ~CARRY;
  ++m_PC;
}

void MOS6502Core::OPCode0x19() {

}

void MOS6502Core::OPCode0x1D() {

}

void MOS6502Core::OPCode0x1E() {

}

/* JSR */
void MOS6502Core::OPCode0x20() {
  uint16_t addr = m_PC + 3;
  StackPush(addr);
  m_PC = m_pMemory->Read(m_PC + 1) | (unsigned)m_pMemory->Read(m_PC + 2) << 8u;
}

void MOS6502Core::OPCode0x21() {

}

void MOS6502Core::OPCode0x24() {

}

void MOS6502Core::OPCode0x25() {

}

void MOS6502Core::OPCode0x26() {

}

void MOS6502Core::OPCode0x28() {

}

void MOS6502Core::OPCode0x29() {

}

void MOS6502Core::OPCode0x2A() {

}

void MOS6502Core::OPCode0x2C() {

}

void MOS6502Core::OPCode0x2D() {

}

void MOS6502Core::OPCode0x2E() {

}

/* BMI relative */
void MOS6502Core::OPCode0x30() {
  if (m_SR & NEGATIVE) {
    m_PC += (int8_t)m_pMemory->Read(++m_PC);
    ++m_PC;
  } else {
    m_PC += 2;
  }
}

void MOS6502Core::OPCode0x31() {

}

void MOS6502Core::OPCode0x35() {

}

void MOS6502Core::OPCode0x36() {

}

/* SEC */
void MOS6502Core::OPCode0x38() {
  m_SR |= CARRY;
  ++m_PC;
}

void MOS6502Core::OPCode0x39() {

}

void MOS6502Core::OPCode0x3D() {

}

void MOS6502Core::OPCode0x3E() {

}

void MOS6502Core::OPCode0x40() {

}

void MOS6502Core::OPCode0x41() {

}

void MOS6502Core::OPCode0x45() {

}

void MOS6502Core::OPCode0x46() {

}

void MOS6502Core::OPCode0x48() {

}

/* ADC */
void MOS6502Core::OPCode0x49() {
  uint8_t val = m_pMemory->Read(++m_PC);
  ++m_PC;

  OPCodeADC(val);
}

void MOS6502Core::OPCode0x4A() {

}

/* JMP abs */
void MOS6502Core::OPCode0x4C() {
  m_PC = m_pMemory->Read(m_PC + 1) | (unsigned)m_pMemory->Read(m_PC + 2) << 8u;
}

void MOS6502Core::OPCode0x4D() {

}

void MOS6502Core::OPCode0x4E() {

}

void MOS6502Core::OPCode0x50() {

}

void MOS6502Core::OPCode0x51() {

}

void MOS6502Core::OPCode0x55() {

}

void MOS6502Core::OPCode0x56() {

}

void MOS6502Core::OPCode0x58() {

}

void MOS6502Core::OPCode0x59() {

}

void MOS6502Core::OPCode0x5D() {

}

void MOS6502Core::OPCode0x5E() {

}

/* RTS */
void MOS6502Core::OPCode0x60() {
  m_PC = StackPull16();
}

void MOS6502Core::OPCode0x61() {

}

void MOS6502Core::OPCode0x65() {

}

void MOS6502Core::OPCode0x66() {

}

void MOS6502Core::OPCode0x68() {

}

void MOS6502Core::OPCode0x69() {

}

void MOS6502Core::OPCode0x6A() {

}

void MOS6502Core::OPCode0x6C() {

}

void MOS6502Core::OPCode0x6D() {

}

void MOS6502Core::OPCode0x6E() {

}

void MOS6502Core::OPCode0x70() {

}

void MOS6502Core::OPCode0x71() {

}

void MOS6502Core::OPCode0x75() {

}

void MOS6502Core::OPCode0x76() {

}

/* SEI impl */
void MOS6502Core::OPCode0x78() {
  m_SR |= INTERRUPT;
  ++m_PC;
}

void MOS6502Core::OPCode0x79() {

}

void MOS6502Core::OPCode0x7D() {

}

void MOS6502Core::OPCode0x7E() {

}

void MOS6502Core::OPCode0x81() {

}

/* STY zpg */
void MOS6502Core::OPCode0x84() {
  m_YR = m_pMemory->Read(++m_PC);
  ++m_PC;

  m_YR ? m_SR &= ~ZERO : m_SR |= ZERO;
}

void MOS6502Core::OPCode0x85() {

}

void MOS6502Core::OPCode0x86() {

}

/* DEY */
void MOS6502Core::OPCode0x88() {
  --m_YR;
  ++m_PC;

  m_YR & 0x80 ? m_SR &= ~NEGATIVE : m_SR |= NEGATIVE;
  m_YR ? m_SR &= ~ZERO : m_SR |= ZERO;
}

void MOS6502Core::OPCode0x8A() {

}

void MOS6502Core::OPCode0x8C() {

}

void MOS6502Core::OPCode0x8D() {

}

void MOS6502Core::OPCode0x8E() {

}

void MOS6502Core::OPCode0x90() {

}

void MOS6502Core::OPCode0x91() {

}

void MOS6502Core::OPCode0x94() {

}

/* STA zpg, X */
void MOS6502Core::OPCode0x95() {
  m_pMemory->Write(m_pMemory->Read(++m_PC), m_AC);
  ++m_PC;
}

void MOS6502Core::OPCode0x96() {

}

void MOS6502Core::OPCode0x98() {

}

void MOS6502Core::OPCode0x99() {

}

/* TXS */
void MOS6502Core::OPCode0x9A() {
  m_SP = m_XR;
  ++m_PC;
}

void MOS6502Core::OPCode0x9D() {

}

/* LDY # */
void MOS6502Core::OPCode0xA0() {
  m_YR = m_pMemory->Read(++m_PC);
  ++m_PC;

  m_YR & 0x80 ? m_SR &= ~NEGATIVE : m_SR |= NEGATIVE;
  m_YR ? m_SR &= ~ZERO : m_SR |= ZERO;
}

void MOS6502Core::OPCode0xA1() {

}

/* LDX # */
void MOS6502Core::OPCode0xA2() {
  m_XR = m_pMemory->Read(++m_PC);
  ++m_PC;

  m_XR & 0x80 ? m_SR |= NEGATIVE : m_SR &= ~NEGATIVE;
  m_XR ? m_SR &= ~ZERO : m_SR |= ZERO;
}

void MOS6502Core::OPCode0xA4() {

}

void MOS6502Core::OPCode0xA5() {

}

void MOS6502Core::OPCode0xA6() {

}

void MOS6502Core::OPCode0xA8() {

}

/* LDA # */
void MOS6502Core::OPCode0xA9() {
  m_AC = m_pMemory->Read(++m_PC);
  ++m_PC;

  m_AC & 0x80 ? m_SR |= NEGATIVE : m_SR &= ~NEGATIVE;
  m_AC ? m_SR &= ~ZERO : m_SR |= ZERO;
}

void MOS6502Core::OPCode0xAA() {

}

void MOS6502Core::OPCode0xAC() {

}

void MOS6502Core::OPCode0xAD() {

}

void MOS6502Core::OPCode0xAE() {

}

/* BCS relative */
void MOS6502Core::OPCode0xB0() {
  if (m_SR & CARRY) {
    m_PC += (int8_t)m_pMemory->Read(++m_PC);
    ++m_PC;
  } else {
    m_PC += 2;
  }
}

void MOS6502Core::OPCode0xB1() {

}

void MOS6502Core::OPCode0xB4() {

}

void MOS6502Core::OPCode0xB5() {

}

void MOS6502Core::OPCode0xB6() {

}

void MOS6502Core::OPCode0xB8() {

}

void MOS6502Core::OPCode0xB9() {

}

void MOS6502Core::OPCode0xBA() {

}

void MOS6502Core::OPCode0xBC() {

}

void MOS6502Core::OPCode0xBD() {

}

void MOS6502Core::OPCode0xBE() {

}

void MOS6502Core::OPCode0xC0() {

}

void MOS6502Core::OPCode0xC1() {

}

void MOS6502Core::OPCode0xC4() {

}

void MOS6502Core::OPCode0xC5() {

}

void MOS6502Core::OPCode0xC6() {

}

/* INY */
void MOS6502Core::OPCode0xC8() {
  ++m_YR;
  ++m_PC;

  m_YR & 0x80 ? m_SR |= NEGATIVE : m_SR &= ~NEGATIVE ;
  m_YR ? m_SR &= ~ZERO : m_SR |= ZERO;
}

void MOS6502Core::OPCode0xC9() {

}

/* DEX impl */
void MOS6502Core::OPCode0xCA() {
  --m_XR;
  ++m_PC;

  m_XR & 0x80 ? m_SR |= NEGATIVE : m_SR &= ~NEGATIVE ;
  m_XR == 0x00 ? m_SR |= ZERO : m_SR &= ~ZERO;
}

void MOS6502Core::OPCode0xCC() {

}

void MOS6502Core::OPCode0xCD() {

}

void MOS6502Core::OPCode0xCE() {

}

void MOS6502Core::OPCode0xD0() {

}

void MOS6502Core::OPCode0xD1() {

}

void MOS6502Core::OPCode0xD5() {

}

void MOS6502Core::OPCode0xD6() {

}

/* CLD impl */
void MOS6502Core::OPCode0xD8() {
  m_SR &= ~DECIMAL;
  ++m_PC;
}

void MOS6502Core::OPCode0xD9() {

}

void MOS6502Core::OPCode0xDD() {

}

void MOS6502Core::OPCode0xDE() {

}

void MOS6502Core::OPCode0xE0() {

}

void MOS6502Core::OPCode0xE1() {

}

void MOS6502Core::OPCode0xE4() {

}

void MOS6502Core::OPCode0xE5() {

}

void MOS6502Core::OPCode0xE6() {

}

void MOS6502Core::OPCode0xE8() {

}

/* SBC */
void MOS6502Core::OPCode0xE9() {
  uint8_t val = m_pMemory->Read(++m_PC);
  ++m_PC;

  OPCodeSBC(val);
}

void MOS6502Core::OPCode0xEA() {

}

void MOS6502Core::OPCode0xEC() {

}

void MOS6502Core::OPCode0xED() {

}

void MOS6502Core::OPCode0xF0() {

}

void MOS6502Core::OPCode0xF1() {

}

void MOS6502Core::OPCode0xF5() {

}

void MOS6502Core::OPCode0xF6() {

}

void MOS6502Core::OPCode0xF8() {

}

void MOS6502Core::OPCode0xF9() {

}

void MOS6502Core::OPCode0xFD() {

}

void MOS6502Core::OPCode0xFE() {

}

void MOS6502Core::OPCodeADC(uint8_t val) {
  uint8_t carry = m_AC & CARRY ? 1 : 0;
  uint16_t result = m_AC + val + carry;

  if (m_SR & DECIMAL) {
    if (m_AC & 0x0F + val & 0x0F + carry > 0x09)
      result += 0x06;

    result & 0x80 ? m_SR |= NEGATIVE : m_SR &= ~NEGATIVE;
    !((m_AC ^ val) & 0x80) && ((m_AC ^ result) & 0x80) ? m_SR |= OVERFLOW : m_SR &= ~OVERFLOW;

    if (carry = result > 0x99)
      result += 0x60;

    carry ? m_SR |= CARRY : m_SR &= ~CARRY;
  } else {
    !((m_AC ^ val) & 0x80) && ((m_AC ^ result) & 0x80) ? m_SR |= OVERFLOW : m_SR &= ~OVERFLOW;
    result > 0xFF ? m_SR |= CARRY : m_SR &= ~CARRY;
    result & 0x80 ? m_SR |= NEGATIVE : m_SR &= ~NEGATIVE;
  }

  result & 0xFF ? m_SR &= ~ZERO : m_SR |= ZERO;

  m_AC = result & 0xFF;
}

void MOS6502Core::OPCodeSBC(uint8_t val) {
  uint8_t carry = m_AC & CARRY ? 1 : 0;

  uint16_t result = m_AC - val - carry;
  result & 0x80 ? m_SR |= NEGATIVE : m_SR &= ~NEGATIVE;
  result ? m_SR &= ~ZERO : m_SR |= ZERO;

  ((m_AC ^ result) & 0x80) && ((m_AC ^ result) & 0x80) ? m_SR |= OVERFLOW : m_SR &= ~OVERFLOW;

  if (m_SR & DECIMAL) {
    if (m_AC & 0x0F - carry < val & 0x0F)
      result -= 0x06;

    if (result > 0x99)
      result -= 0x60;
  }

  result < 0x100 ? m_SR |= CARRY : m_SR &= ~CARRY;
  m_AC = result & 0xFF;
}

void MOS6502Core::OPCodesASL(uint16_t address) {
  uint8_t result = m_pMemory->Read(address) << 1;

}

void MOS6502Core::StackPush(uint16_t pushval) {
  m_pMemory->Write(STACK_BASE + m_SP, (pushval >> 8) & 0xFF);
  m_pMemory->Write(STACK_BASE + ((m_SP - 1) & 0xFF), pushval & 0xFF);
  m_SP -= 2;
}

void MOS6502Core::StackPush(uint8_t pushval) {
  m_pMemory->Write(STACK_BASE + m_SP--, pushval);
}

uint16_t MOS6502Core::StackPull16() {
  uint16_t temp = m_pMemory->Read(STACK_BASE + ((m_SP + 1) & 0xFF)) | ((uint16_t)m_pMemory->Read(STACK_BASE + ((m_SP + 2) & 0xFF)) << 8);
  m_SP += 2;
  return(temp);
}

uint8_t MOS6502Core::StackPull8() {
  return (m_pMemory->Read(STACK_BASE + ++m_SP));
}
