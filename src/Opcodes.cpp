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
  m_OPCodes[0x11] = &MOS6502Core::OPCode0x11;
  m_OPCodes[0x15] = &MOS6502Core::OPCode0x15;
  m_OPCodes[0x16] = &MOS6502Core::OPCode0x16;
  m_OPCodes[0x18] = &MOS6502Core::OPCode0x18;
  m_OPCodes[0x19] = &MOS6502Core::OPCode0x19;
  m_OPCodes[0x1D] = &MOS6502Core::OPCode0x1D;
  m_OPCodes[0x1E] = &MOS6502Core::OPCode0x1E;

  m_OPCodes[0x20] = &MOS6502Core::OPCode0x20;
  m_OPCodes[0x21] = &MOS6502Core::OPCode0x21;
  m_OPCodes[0x24] = &MOS6502Core::OPCode0x24;
  m_OPCodes[0x25] = &MOS6502Core::OPCode0x25;
  m_OPCodes[0x26] = &MOS6502Core::OPCode0x26;
  m_OPCodes[0x28] = &MOS6502Core::OPCode0x28;
  m_OPCodes[0x29] = &MOS6502Core::OPCode0x29;
  m_OPCodes[0x2A] = &MOS6502Core::OPCode0x2A;
  m_OPCodes[0x2C] = &MOS6502Core::OPCode0x2C;
  m_OPCodes[0x2D] = &MOS6502Core::OPCode0x2D;
  m_OPCodes[0x2E] = &MOS6502Core::OPCode0x2E;

  m_OPCodes[0x30] = &MOS6502Core::OPCode0x30;
  m_OPCodes[0x31] = &MOS6502Core::OPCode0x31;
  m_OPCodes[0x35] = &MOS6502Core::OPCode0x35;
  m_OPCodes[0x36] = &MOS6502Core::OPCode0x36;
  m_OPCodes[0x38] = &MOS6502Core::OPCode0x38;
  m_OPCodes[0x39] = &MOS6502Core::OPCode0x39;
  m_OPCodes[0x3D] = &MOS6502Core::OPCode0x3D;
  m_OPCodes[0x3E] = &MOS6502Core::OPCode0x3E;

  m_OPCodes[0x40] = &MOS6502Core::OPCode0x40;
  m_OPCodes[0x41] = &MOS6502Core::OPCode0x41;
  m_OPCodes[0x45] = &MOS6502Core::OPCode0x45;
  m_OPCodes[0x46] = &MOS6502Core::OPCode0x46;
  m_OPCodes[0x48] = &MOS6502Core::OPCode0x48;
  m_OPCodes[0x49] = &MOS6502Core::OPCode0x49;
  m_OPCodes[0x4A] = &MOS6502Core::OPCode0x4A;
  m_OPCodes[0x4C] = &MOS6502Core::OPCode0x4C;
  m_OPCodes[0x4D] = &MOS6502Core::OPCode0x4D;
  m_OPCodes[0x4E] = &MOS6502Core::OPCode0x4E;

  m_OPCodes[0x50] = &MOS6502Core::OPCode0x50;
  m_OPCodes[0x51] = &MOS6502Core::OPCode0x51;
  m_OPCodes[0x55] = &MOS6502Core::OPCode0x55;
  m_OPCodes[0x56] = &MOS6502Core::OPCode0x56;
  m_OPCodes[0x58] = &MOS6502Core::OPCode0x58;
  m_OPCodes[0x59] = &MOS6502Core::OPCode0x59;
  m_OPCodes[0x5D] = &MOS6502Core::OPCode0x5D;
  m_OPCodes[0x5E] = &MOS6502Core::OPCode0x5E;

  m_OPCodes[0x60] = &MOS6502Core::OPCode0x60;
  m_OPCodes[0x61] = &MOS6502Core::OPCode0x61;
  m_OPCodes[0x65] = &MOS6502Core::OPCode0x65;
  m_OPCodes[0x66] = &MOS6502Core::OPCode0x66;
  m_OPCodes[0x68] = &MOS6502Core::OPCode0x68;
  m_OPCodes[0x69] = &MOS6502Core::OPCode0x69;
  m_OPCodes[0x6A] = &MOS6502Core::OPCode0x6A;
  m_OPCodes[0x6C] = &MOS6502Core::OPCode0x6C;
  m_OPCodes[0x6D] = &MOS6502Core::OPCode0x6D;
  m_OPCodes[0x6E] = &MOS6502Core::OPCode0x6E;

  m_OPCodes[0x70] = &MOS6502Core::OPCode0x70;
  m_OPCodes[0x71] = &MOS6502Core::OPCode0x71;
  m_OPCodes[0x75] = &MOS6502Core::OPCode0x75;
  m_OPCodes[0x76] = &MOS6502Core::OPCode0x76;
  m_OPCodes[0x78] = &MOS6502Core::OPCode0x78;
  m_OPCodes[0x79] = &MOS6502Core::OPCode0x79;
  m_OPCodes[0x7D] = &MOS6502Core::OPCode0x7D;
  m_OPCodes[0x7E] = &MOS6502Core::OPCode0x7E;

  m_OPCodes[0x81] = &MOS6502Core::OPCode0x81;
  m_OPCodes[0x84] = &MOS6502Core::OPCode0x84;
  m_OPCodes[0x85] = &MOS6502Core::OPCode0x85;
  m_OPCodes[0x86] = &MOS6502Core::OPCode0x86;
  m_OPCodes[0x88] = &MOS6502Core::OPCode0x88;
  m_OPCodes[0x8A] = &MOS6502Core::OPCode0x8A;
  m_OPCodes[0x8C] = &MOS6502Core::OPCode0x8C;
  m_OPCodes[0x8D] = &MOS6502Core::OPCode0x8D;
  m_OPCodes[0x8E] = &MOS6502Core::OPCode0x8E;

  m_OPCodes[0x90] = &MOS6502Core::OPCode0x90;
  m_OPCodes[0x91] = &MOS6502Core::OPCode0x91;
  m_OPCodes[0x94] = &MOS6502Core::OPCode0x94;
  m_OPCodes[0x95] = &MOS6502Core::OPCode0x95;
  m_OPCodes[0x96] = &MOS6502Core::OPCode0x96;
  m_OPCodes[0x98] = &MOS6502Core::OPCode0x98;
  m_OPCodes[0x99] = &MOS6502Core::OPCode0x99;
  m_OPCodes[0x9A] = &MOS6502Core::OPCode0x9A;
  m_OPCodes[0x9D] = &MOS6502Core::OPCode0x9D;

  m_OPCodes[0xA0] = &MOS6502Core::OPCode0xA0;
  m_OPCodes[0xA1] = &MOS6502Core::OPCode0xA1;
  m_OPCodes[0xA2] = &MOS6502Core::OPCode0xA2;
  m_OPCodes[0xA4] = &MOS6502Core::OPCode0xA4;
  m_OPCodes[0xA5] = &MOS6502Core::OPCode0xA5;
  m_OPCodes[0xA6] = &MOS6502Core::OPCode0xA6;
  m_OPCodes[0xA8] = &MOS6502Core::OPCode0xA8;
  m_OPCodes[0xA9] = &MOS6502Core::OPCode0xA9;
  m_OPCodes[0xAA] = &MOS6502Core::OPCode0xAA;
  m_OPCodes[0xAC] = &MOS6502Core::OPCode0xAC;
  m_OPCodes[0xAD] = &MOS6502Core::OPCode0xAD;
  m_OPCodes[0xAE] = &MOS6502Core::OPCode0xAE;

  m_OPCodes[0xB0] = &MOS6502Core::OPCode0xB0;
  m_OPCodes[0xB1] = &MOS6502Core::OPCode0xB1;
  m_OPCodes[0xB4] = &MOS6502Core::OPCode0xB4;
  m_OPCodes[0xB5] = &MOS6502Core::OPCode0xB5;
  m_OPCodes[0xB6] = &MOS6502Core::OPCode0xB6;
  m_OPCodes[0xB8] = &MOS6502Core::OPCode0xB8;
  m_OPCodes[0xB9] = &MOS6502Core::OPCode0xB9;
  m_OPCodes[0xBA] = &MOS6502Core::OPCode0xBA;
  m_OPCodes[0xBC] = &MOS6502Core::OPCode0xBC;
  m_OPCodes[0xBD] = &MOS6502Core::OPCode0xBD;
  m_OPCodes[0xBE] = &MOS6502Core::OPCode0xBE;

  m_OPCodes[0xC0] = &MOS6502Core::OPCode0xC0;
  m_OPCodes[0xC1] = &MOS6502Core::OPCode0xC1;
  m_OPCodes[0xC4] = &MOS6502Core::OPCode0xC4;
  m_OPCodes[0xC5] = &MOS6502Core::OPCode0xC5;
  m_OPCodes[0xC6] = &MOS6502Core::OPCode0xC6;
  m_OPCodes[0xC8] = &MOS6502Core::OPCode0xC8;
  m_OPCodes[0xC9] = &MOS6502Core::OPCode0xC9;
  m_OPCodes[0xCA] = &MOS6502Core::OPCode0xCA;
  m_OPCodes[0xCC] = &MOS6502Core::OPCode0xCC;
  m_OPCodes[0xCD] = &MOS6502Core::OPCode0xCD;
  m_OPCodes[0xCE] = &MOS6502Core::OPCode0xCE;

  m_OPCodes[0xD0] = &MOS6502Core::OPCode0xD0;
  m_OPCodes[0xD1] = &MOS6502Core::OPCode0xD1;
  m_OPCodes[0xD5] = &MOS6502Core::OPCode0xD5;
  m_OPCodes[0xD6] = &MOS6502Core::OPCode0xD6;
  m_OPCodes[0xD8] = &MOS6502Core::OPCode0xD8;
  m_OPCodes[0xD9] = &MOS6502Core::OPCode0xD9;
  m_OPCodes[0xDD] = &MOS6502Core::OPCode0xDD;
  m_OPCodes[0xDE] = &MOS6502Core::OPCode0xDE;

  m_OPCodes[0xE0] = &MOS6502Core::OPCode0xE0;
  m_OPCodes[0xE1] = &MOS6502Core::OPCode0xE1;
  m_OPCodes[0xE4] = &MOS6502Core::OPCode0xE4;
  m_OPCodes[0xE5] = &MOS6502Core::OPCode0xE5;
  m_OPCodes[0xE6] = &MOS6502Core::OPCode0xE6;
  m_OPCodes[0xE8] = &MOS6502Core::OPCode0xE8;
  m_OPCodes[0xE9] = &MOS6502Core::OPCode0xE9;
  m_OPCodes[0xEA] = &MOS6502Core::OPCode0xEA;
  m_OPCodes[0xEC] = &MOS6502Core::OPCode0xEC;
  m_OPCodes[0xED] = &MOS6502Core::OPCode0xED;
  m_OPCodes[0xEE] = &MOS6502Core::OPCode0xEE;

  m_OPCodes[0xF0] = &MOS6502Core::OPCode0xF0;
  m_OPCodes[0xF1] = &MOS6502Core::OPCode0xF1;
  m_OPCodes[0xF5] = &MOS6502Core::OPCode0xF5;
  m_OPCodes[0xF6] = &MOS6502Core::OPCode0xF6;
  m_OPCodes[0xF8] = &MOS6502Core::OPCode0xF8;
  m_OPCodes[0xF9] = &MOS6502Core::OPCode0xF9;
  m_OPCodes[0xFD] = &MOS6502Core::OPCode0xFD;
  m_OPCodes[0xFE] = &MOS6502Core::OPCode0xFE;
}

void MOS6502Core::OPCodeInvalid() {
  std::cout << "INVALID OPCODE!!" << std::endl;
}

/* BRK */
void MOS6502Core::OPCode0x00() {
  StackPush(m_PC);
  StackPush((uint8_t)(m_SR | BREAK));
  m_PC = m_pMemory->Read(BRK_VECTOR) | (m_pMemory->Read(BRK_VECTOR + 1));
}

/* ORA x, Indirect */
void MOS6502Core::OPCode0x01() {
  uint16_t address = m_pMemory->Read(++m_PC) + m_XR;
  address |= (m_pMemory->Read(address + 1u)) << 8u;

  OPCodesORA(m_pMemory->Read(address));
  ++m_PC;
}

/* ORA, ZPG */
void MOS6502Core::OPCode0x05() {
  OPCodesORA(m_pMemory->Read(++m_PC));
  ++m_PC;
}

/* ASL, ZPG */
void MOS6502Core::OPCode0x06() {
  OPCodesASL(m_pMemory->Read(++m_PC));
  ++m_PC;
}

/* PHP */
void MOS6502Core::OPCode0x08() {
  StackPush(m_SR);
  ++m_PC;
}

/* ORA # */
void MOS6502Core::OPCode0x09() {
  OPCodesORA(++m_PC);
  ++m_PC;
}

/* ASL A */
void MOS6502Core::OPCode0x0A() {
  m_AC & 0x80 ? m_SR |= CARRY : m_SR &= ~CARRY;
  m_AC <<= 1;
  ++m_PC;

  m_AC & 0x80 ? m_SR |= NEGATIVE : m_SR &=~NEGATIVE;
  m_AC ? m_SR &= ~ZERO : m_SR |= ZERO;
}

/* ORA ABS */
void MOS6502Core::OPCode0x0D() {
  OPCodesORA(m_pMemory->Read(m_PC + 1) | (m_pMemory->Read(m_PC + 2)) << 8u);
  m_PC += 3;
}

/* ASL ABS */
void MOS6502Core::OPCode0x0E() {
  OPCodesASL(m_pMemory->Read(m_PC + 1) | (m_pMemory->Read(m_PC + 2)) << 8u);
  m_PC += 3;
}

/* BPL relative */
void MOS6502Core::OPCode0x10() {
  (m_SR & NEGATIVE) == 0x00 ? m_PC += (int8_t)m_pMemory->Read(++m_PC) : ++m_PC;
  ++m_PC;
}

/* ORA Y-Indirect */
void MOS6502Core::OPCode0x11() {
  uint16_t address = m_pMemory->Read(++m_PC) + m_YR;
  address |= (m_pMemory->Read(address + 1u)) << 8u;

  OPCodesORA(m_pMemory->Read(address));
  ++m_PC;
}

/* ORA ZPG, X */
void MOS6502Core::OPCode0x15() {
  OPCodesORA(m_pMemory->Read(++m_PC) + m_XR);
  ++m_PC;
}

/* ASL ZPG, X */
void MOS6502Core::OPCode0x16() {
  OPCodesASL(m_pMemory->Read(++m_PC) + m_XR);
  ++m_PC;
}

/* CLC */
void MOS6502Core::OPCode0x18() {
  m_SR &= ~CARRY;
  ++m_PC;
}

/* ORA ABS Y */
void MOS6502Core::OPCode0x19() {
  OPCodesORA((m_pMemory->Read(m_PC + 1) | (m_pMemory->Read(m_PC + 2)) << 8u) + m_YR);
  m_PC += 3;
}

/* ORA ABS X */
void MOS6502Core::OPCode0x1D() {
  OPCodesORA((m_pMemory->Read(m_PC + 1) | (m_pMemory->Read(m_PC + 2)) << 8u) + m_XR);
  m_PC += 3;
}

/* ASL ABS + X */
void MOS6502Core::OPCode0x1E() {
  uint16_t address = m_pMemory->Read(m_PC + 1) | (m_pMemory->Read(m_PC + 2)) << 8u;
  OPCodesASL(address + m_XR);
  m_PC += 3;
}

/* JSR */
void MOS6502Core::OPCode0x20() {
  uint16_t addr = m_PC + 3;
  StackPush(addr);
  m_PC = m_pMemory->Read(m_PC + 1) | (unsigned)m_pMemory->Read(m_PC + 2) << 8u;
}

/* AND X-Indirect */
void MOS6502Core::OPCode0x21() {
  uint16_t address = m_pMemory->Read(++m_PC) + m_XR;
  address |= (m_pMemory->Read(address + 1u)) << 8u;

  OPCodesAND(m_pMemory->Read(address));
  ++m_PC;
}

/* BIT ZPG */
void MOS6502Core::OPCode0x24() {
  OPCodesBIT(m_pMemory->Read(++m_PC));
  ++m_PC;
}

/* AND ZPG */
void MOS6502Core::OPCode0x25() {
  OPCodesAND(m_pMemory->Read(++m_PC));
  ++m_PC;
}

/* ROL ZPG */
void MOS6502Core::OPCode0x26() {
  OPCodesROL(m_pMemory->Read(++m_PC));
  ++m_PC;
}

/* PLP */
void MOS6502Core::OPCode0x28() {
  m_SR = StackPull8() | CONSTANT;
  ++m_PC;
}

/* AND # */
void MOS6502Core::OPCode0x29() {
  OPCodesAND(++m_PC);
  ++m_PC;
}

/* ROL A */
void MOS6502Core::OPCode0x2A() {
  uint16_t val = m_AC;

  if (m_SR & CARRY) val |= 0x01;
  val > 0xFF ? m_SR |= CARRY : m_SR &= ~CARRY;
  val & 0x80 ? m_SR |= NEGATIVE : m_SR &= ~NEGATIVE;
  val ? m_SR &= ~ZERO : m_SR |= ZERO;

  m_AC = val & 0xFF;
  ++m_PC;
}

/* BIT ABS */
void MOS6502Core::OPCode0x2C() {
  uint16_t address = m_pMemory->Read(m_PC + 1) | (m_pMemory->Read(m_PC + 2)) << 8u;
  OPCodesBIT(address);
  m_PC += 3;
}

/* AND ABS */
void MOS6502Core::OPCode0x2D() {
  OPCodesAND( m_pMemory->Read(m_PC + 1) | m_pMemory->Read(m_PC + 2) << 8u);
  m_PC += 3;
}

/* ROL # */
void MOS6502Core::OPCode0x2E() {
  OPCodesROL(++m_PC);
  ++m_PC;
}

/* BMI relative */
void MOS6502Core::OPCode0x30() {
  m_SR & NEGATIVE ? m_PC += (int8_t)m_pMemory->Read(++m_PC) : ++m_PC;
  ++m_PC;
}

/* AND Y-Indirect */
void MOS6502Core::OPCode0x31() {
  uint16_t address = m_pMemory->Read(++m_PC) + m_YR;
  address |= (m_pMemory->Read(address + 1u)) << 8u;

  OPCodesAND(m_pMemory->Read(address));
  ++m_PC;
}

/* AND zpg, X */
void MOS6502Core::OPCode0x35() {
  OPCodesAND(m_pMemory->Read(++m_PC) + m_XR);
  ++m_PC;
}

/* ROL zpg, X */
void MOS6502Core::OPCode0x36() {
  OPCodesROL(m_pMemory->Read(++m_PC) + m_XR);
  ++m_PC;
}

/* SEC */
void MOS6502Core::OPCode0x38() {
  m_SR |= CARRY;
  ++m_PC;
}

/* AND abs, Y */
void MOS6502Core::OPCode0x39() {
  OPCodesAND((m_pMemory->Read(m_PC + 1) | m_pMemory->Read(m_PC + 2) << 8u) + m_YR);
  m_PC += 3;
}

/* AND abs, X */
void MOS6502Core::OPCode0x3D() {
  OPCodesAND((m_pMemory->Read(m_PC + 1) | m_pMemory->Read(m_PC + 2) << 8u) + m_XR);
  m_PC += 3;
}

/* ROL abs, X */
void MOS6502Core::OPCode0x3E() {
  OPCodesROL((m_pMemory->Read(m_PC + 1) | m_pMemory->Read(m_PC + 2) << 8u) + m_XR);
  m_PC += 3;
}

/* RTI */
void MOS6502Core::OPCode0x40() {
  m_SR = StackPull8() | CONSTANT;
  m_PC = StackPull16();
}

/* EOR X-Indirect */
void MOS6502Core::OPCode0x41() {
  uint16_t address = m_pMemory->Read(++m_PC) + m_XR;
  address |= (m_pMemory->Read(address + 1u)) << 8u;

  OPCodesEOR(m_pMemory->Read(address));
  ++m_PC;
}

/* EOR zpg */
void MOS6502Core::OPCode0x45() {
  OPCodesEOR(m_pMemory->Read(++m_PC));
  ++m_PC;
}

/* LSR zpg */
void MOS6502Core::OPCode0x46() {
  OPCodesLSR(m_pMemory->Read(++m_PC));
  ++m_PC;
}

/* PHA */
void MOS6502Core::OPCode0x48() {
  StackPush(m_AC);
  ++m_PC;
}

/* EOR # */
void MOS6502Core::OPCode0x49() {
  OPCodesEOR(++m_PC);
  ++m_PC;
}

/* LSR A */
void MOS6502Core::OPCode0x4A() {
  m_AC & 0x01 ? m_SR |= CARRY : m_SR &= ~CARRY;

  m_AC >>= 1;

  m_AC & 0x80u ? m_SR |= NEGATIVE : m_SR &=~NEGATIVE;
  m_AC ? m_SR &= ~ZERO : m_SR |= ZERO;

  ++m_PC;
}

/* JMP abs */
void MOS6502Core::OPCode0x4C() {
  m_PC = m_pMemory->Read(m_PC + 1) | (unsigned)m_pMemory->Read(m_PC + 2) << 8u;
}

/* EOR abs */
void MOS6502Core::OPCode0x4D() {
  OPCodesEOR(m_pMemory->Read(m_PC + 1) | m_pMemory->Read(m_PC + 2) << 8u);
  m_PC += 3;
}

/* LSR abs */
void MOS6502Core::OPCode0x4E() {
  OPCodesLSR(m_pMemory->Read(m_PC + 1) | m_pMemory->Read(m_PC + 2) << 8u);
  m_PC += 3;
}

/* BVC */
void MOS6502Core::OPCode0x50() {
  (m_SR & OVERFLOW) == 0x00 ? m_PC += (int8_t)m_pMemory->Read(++m_PC) : ++m_PC;
  ++m_PC;
}

/* EOR Y-Indirect */
void MOS6502Core::OPCode0x51() {
  uint16_t address = m_pMemory->Read(++m_PC) + m_YR;
  address |= (m_pMemory->Read(address + 1u)) << 8u;

  OPCodesEOR(m_pMemory->Read(address));
  ++m_PC;
}

/* EOR zpg, X*/
void MOS6502Core::OPCode0x55() {
  OPCodesEOR(m_pMemory->Read(++m_PC) + m_XR);
  ++m_PC;
}

/* LSR zpg, X */
void MOS6502Core::OPCode0x56() {
  OPCodesLSR(m_pMemory->Read(++m_PC) + m_XR);
  ++m_PC;
}

/* CLI */
void MOS6502Core::OPCode0x58() {
  m_SR &= ~INTERRUPT;
  ++m_PC;
}

/* EOR abs, Y */
void MOS6502Core::OPCode0x59() {
  OPCodesEOR((m_pMemory->Read(m_PC + 1) | m_pMemory->Read(m_PC + 2) << 8u) + m_YR);
  m_PC += 3;
}

/* EOR abs, X */
void MOS6502Core::OPCode0x5D() {
  OPCodesEOR((m_pMemory->Read(m_PC + 1) | m_pMemory->Read(m_PC + 2) << 8u) + m_XR);
  m_PC += 3;
}

/* LSR abs, X */
void MOS6502Core::OPCode0x5E() {
  OPCodesLSR((m_pMemory->Read(m_PC + 1) | m_pMemory->Read(m_PC + 2) << 8u) + m_XR);
  m_PC += 3;
}

/* RTS */
void MOS6502Core::OPCode0x60() {
  m_PC = StackPull16();
}

/* ADC X-Indirect */
void MOS6502Core::OPCode0x61() {
  uint16_t address = m_pMemory->Read(++m_PC) + m_XR;
  address |= (m_pMemory->Read(address + 1u)) << 8u;

  OPCodesADC(m_pMemory->Read(address));
  ++m_PC;
}

/* ADC zpg */
void MOS6502Core::OPCode0x65() {
  OPCodesADC(m_pMemory->Read(++m_PC));
  ++m_PC;
}

/* ROR zpg */
void MOS6502Core::OPCode0x66() {
  OPCodesROR(m_pMemory->Read(++m_PC));
  ++m_PC;
}

/* PLA */
void MOS6502Core::OPCode0x68() {
  m_AC = StackPull8();
  ++m_PC;

  m_AC & 0x80u ? m_SR |= NEGATIVE : m_SR &= ~NEGATIVE;
  m_AC ? m_SR &= ~ZERO : m_SR |= ZERO;
}

/* ADC # */
void MOS6502Core::OPCode0x69() {
  OPCodesADC(++m_PC);
  ++m_PC;
}

/* ROR A */
void MOS6502Core::OPCode0x6A() {
  uint16_t val = m_AC;

  if (m_SR & CARRY) val |= 0x100;
  val & 0x01 ? m_SR |= CARRY : m_SR &= ~CARRY;

  val >>= 1u;

  val & 0x80 ? m_SR |= NEGATIVE : m_SR &= ~NEGATIVE;
  val ? m_SR &= ~ZERO : m_SR |= ZERO;

  m_AC = val & 0xFF;

  ++m_PC;
}

/* JMP Indirect */
void MOS6502Core::OPCode0x6C() {
  m_PC = m_pMemory->Read(m_pMemory->Read(m_PC + 1) | (unsigned)m_pMemory->Read(m_PC + 2) << 8u);
}

/* ADC abs */
void MOS6502Core::OPCode0x6D() {
  OPCodesADC(m_pMemory->Read(m_PC + 1) | m_pMemory->Read(m_PC + 2) << 8u);
  m_PC += 3;
}

/* ROR abs */
void MOS6502Core::OPCode0x6E() {
  OPCodesROR(m_pMemory->Read(m_PC + 1) | m_pMemory->Read(m_PC + 2) << 8u);
  m_PC += 3;
}

/* BVS */
void MOS6502Core::OPCode0x70() {
  m_SR & OVERFLOW ? m_PC += (int8_t)m_pMemory->Read(++m_PC) : ++m_PC;
  ++m_PC;
}

/* ADC Y-Indirect */
void MOS6502Core::OPCode0x71() {
  uint16_t address = m_pMemory->Read(++m_PC) + m_YR;
  address |= (m_pMemory->Read(address + 1u)) << 8u;

  OPCodesADC(m_pMemory->Read(address));
  ++m_PC;
}

/* ADC zpg, X */
void MOS6502Core::OPCode0x75() {
  OPCodesADC(m_pMemory->Read(++m_PC) + m_XR);
  ++m_PC;
}

/* ROR zpg, X */
void MOS6502Core::OPCode0x76() {
  OPCodesROR(m_pMemory->Read(++m_PC) + m_XR);
  ++m_PC;
}

/* SEI impl */
void MOS6502Core::OPCode0x78() {
  m_SR |= INTERRUPT;
  ++m_PC;
}

/* ADC abs, Y */
void MOS6502Core::OPCode0x79() {
  OPCodesADC((m_pMemory->Read(m_PC + 1) | m_pMemory->Read(m_PC + 2) << 8u) + m_YR);
  m_PC += 3;
}

/* ADC abs, X */
void MOS6502Core::OPCode0x7D() {
  OPCodesADC((m_pMemory->Read(m_PC + 1) | m_pMemory->Read(m_PC + 2) << 8u) + m_XR);
  m_PC += 3;
}

/* ROR abs, X */
void MOS6502Core::OPCode0x7E() {
  OPCodesROR((m_pMemory->Read(m_PC + 1) | m_pMemory->Read(m_PC + 2) << 8u) + m_XR);
  m_PC += 3;
}

/* STA X-Indirect */
void MOS6502Core::OPCode0x81() {
  uint16_t address = m_pMemory->Read(++m_PC) + m_XR;
  address |= (m_pMemory->Read(address + 1u)) << 8u;

  OPCodesSTA(m_pMemory->Read(address));
  ++m_PC;
}

/* STY zpg */
void MOS6502Core::OPCode0x84() {
  OPCodesSTY(m_pMemory->Read(++m_PC));
  ++m_PC;
}

/* STA zpg */
void MOS6502Core::OPCode0x85() {
  OPCodesSTA(m_pMemory->Read(++m_PC));
  ++m_PC;
}

/* STX zpg */
void MOS6502Core::OPCode0x86() {
  OPCodesSTX(m_pMemory->Read(++m_PC));
  ++m_PC;
}

/* DEY */
void MOS6502Core::OPCode0x88() {
  --m_YR;
  ++m_PC;

  m_YR & 0x80 ? m_SR |= NEGATIVE : m_SR &=~NEGATIVE;
  m_YR ? m_SR &= ~ZERO : m_SR |= ZERO;
}

/* TXA */
void MOS6502Core::OPCode0x8A() {
  m_AC = m_XR;
  ++m_PC;

  m_AC & 0x80 ? m_SR |= NEGATIVE : m_SR &=~NEGATIVE;
  m_AC ? m_SR &= ~ZERO : m_SR |= ZERO;
}

/* STY abs */
void MOS6502Core::OPCode0x8C() {
  OPCodesSTY(m_pMemory->Read(m_PC + 1) | m_pMemory->Read(m_PC + 2) << 8u);
  m_PC += 3;
}

/* STA abs */
void MOS6502Core::OPCode0x8D() {
  OPCodesSTA(m_pMemory->Read(m_PC + 1) | m_pMemory->Read(m_PC + 2) << 8u);
  m_PC += 3;
}

/* STX abs */
void MOS6502Core::OPCode0x8E() {
  OPCodesSTX(m_pMemory->Read(m_PC + 1) | m_pMemory->Read(m_PC + 2) << 8u);
  m_PC += 3;
}

/* BCC */
void MOS6502Core::OPCode0x90() {
  (m_SR & CARRY) == 0x00 ? m_PC += (int8_t)m_pMemory->Read(++m_PC) : ++m_PC;
  ++m_PC;
}

/* STA Y-Indirect */
void MOS6502Core::OPCode0x91() {
  uint16_t address = m_pMemory->Read(++m_PC) + m_YR;
  address |= (m_pMemory->Read(address + 1u)) << 8u;

  OPCodesSTA(m_pMemory->Read(address));
  ++m_PC;
}

/* STY zpg, X */
void MOS6502Core::OPCode0x94() {
  OPCodesSTY(m_pMemory->Read(++m_PC) + m_XR);
  ++m_PC;
}

/* STA zpg, X */
void MOS6502Core::OPCode0x95() {
  OPCodesSTA(m_pMemory->Read(++m_PC) + m_XR);
  ++m_PC;
}

/* STX zpg, Y */
void MOS6502Core::OPCode0x96() {
  OPCodesSTX(m_pMemory->Read(++m_PC) + m_YR);
  ++m_PC;
}

/* TYA */
void MOS6502Core::OPCode0x98() {
  m_AC = m_YR;

  m_AC & 0x80 ? m_SR |= NEGATIVE : m_SR &= ~NEGATIVE;
  m_AC ? m_SR &= ~ZERO : m_SR |= ZERO;
}

/* STA abs, Y */
void MOS6502Core::OPCode0x99() {
  OPCodesSTA((m_pMemory->Read(m_PC + 1) | m_pMemory->Read(m_PC + 1) << 8u) + m_YR);
  m_PC += 3;
}

/* TXS */
void MOS6502Core::OPCode0x9A() {
  m_SP = m_XR;
  ++m_PC;
}

/* STA abs, X */
void MOS6502Core::OPCode0x9D() {
  OPCodesSTA((m_pMemory->Read(m_PC + 1) | m_pMemory->Read(m_PC + 2) << 8u) + m_XR);
  m_PC += 3;
}

/* LDY # */
void MOS6502Core::OPCode0xA0() {
  OPCodesLDY(++m_PC);
  ++m_PC;
}

/* LDA X-Indirect */
void MOS6502Core::OPCode0xA1() {
  uint16_t address = m_pMemory->Read(++m_PC) + m_XR;
  address |= (m_pMemory->Read(address + 1u)) << 8u;

  OPCodesLDA(m_pMemory->Read(address));
  ++m_PC;
}

/* LDX # */
void MOS6502Core::OPCode0xA2() {
  OPCodesLDX(++m_PC);
  ++m_PC;
}

/* LDY zpg */
void MOS6502Core::OPCode0xA4() {
  OPCodesLDY(m_pMemory->Read(++m_PC));
  ++m_PC;
}

/* LDA zpg */
void MOS6502Core::OPCode0xA5() {
  OPCodesLDA(m_pMemory->Read(++m_PC));
  ++m_PC;
}

/* LDX zpg */
void MOS6502Core::OPCode0xA6() {
  OPCodesLDX(m_pMemory->Read(++m_PC));
  ++m_PC;
}

/* TAY */
void MOS6502Core::OPCode0xA8() {
  m_YR = m_AC;
  ++m_PC;

  m_YR & 0x80 ? m_SR |= NEGATIVE : m_SR &= ~NEGATIVE;
  m_YR ? m_SR &= ~ZERO : m_SR |= ZERO;
}

/* LDA # */
void MOS6502Core::OPCode0xA9() {
  OPCodesLDA(++m_PC);
  ++m_PC;
}

/* TAX */
void MOS6502Core::OPCode0xAA() {
  m_XR = m_AC;
  ++m_PC;

  m_XR & 0x80 ? m_SR |= NEGATIVE : m_SR &= ~NEGATIVE;
  m_XR ? m_SR &= ~ZERO : m_SR |= ZERO;
}

/* LDY ABS */
void MOS6502Core::OPCode0xAC() {
  OPCodesLDY(m_pMemory->Read(m_PC + 1) | m_pMemory->Read(m_PC + 2) << 8u);
  m_PC += 3;
}

/* LDA ABS */
void MOS6502Core::OPCode0xAD() {
  OPCodesLDA(m_pMemory->Read(m_PC + 1) | m_pMemory->Read(m_PC + 2) << 8u);
  m_PC += 3;
}

/* LDX ABS */
void MOS6502Core::OPCode0xAE() {
  OPCodesLDX(m_pMemory->Read(m_PC + 1) | m_pMemory->Read(m_PC + 2) << 8u);
  m_PC += 3;
}

/* BCS relative */
void MOS6502Core::OPCode0xB0() {
  m_SR & CARRY ? m_PC += (int8_t)m_pMemory->Read(++m_PC) : ++m_PC;
  ++m_PC;
}

/* LDA Y-Indirect */
void MOS6502Core::OPCode0xB1() {
  uint16_t address = m_pMemory->Read(++m_PC) + m_YR;
  address |= (m_pMemory->Read(address + 1u)) << 8u;

  OPCodesLDA(m_pMemory->Read(address));
  ++m_PC;
}

/* LDY zpg, X */
void MOS6502Core::OPCode0xB4() {
  OPCodesLDY(m_pMemory->Read(++m_PC) + m_XR);
  ++m_PC;
}

/* LDA zpg, X */
void MOS6502Core::OPCode0xB5() {
  OPCodesLDA(m_pMemory->Read(++m_PC) + m_XR);
  ++m_PC;
}

/* LDX zpg, Y */
void MOS6502Core::OPCode0xB6() {
  OPCodesLDX(m_pMemory->Read(++m_PC) + m_YR);
  ++m_PC;
}

/* CLV */
void MOS6502Core::OPCode0xB8() {
  m_SR &= ~OVERFLOW;
  ++m_PC;
}

/* LDA abs, Y */
void MOS6502Core::OPCode0xB9() {
  OPCodesLDA((m_pMemory->Read(m_PC + 1) | m_pMemory->Read(m_PC + 2) << 8u) + m_YR);
  m_PC += 3;
}

/* TSX */
void MOS6502Core::OPCode0xBA() {
  m_SP = m_XR;
  ++m_PC;

  m_XR & 0x80 ? m_SR |= NEGATIVE : m_SR &= ~NEGATIVE;
  m_XR ? m_SR &= ~ZERO : m_SR |= ZERO;
}

/* LDY abs, X */
void MOS6502Core::OPCode0xBC() {
  OPCodesLDY((m_pMemory->Read(m_PC + 1) | m_pMemory->Read(m_PC + 2) << 8u) + m_XR);
  ++m_PC;
}

/* LDA abs, X */
void MOS6502Core::OPCode0xBD() {
  OPCodesLDA((m_pMemory->Read(m_PC + 1) | m_pMemory->Read(m_PC + 2) << 8u) + m_XR);
  ++m_PC;
}

/* LDX abs, Y */
void MOS6502Core::OPCode0xBE() {
  OPCodesLDX((m_pMemory->Read(m_PC + 1) | m_pMemory->Read(m_PC + 2) << 8u) + m_YR);
  ++m_PC;
}

/* CPY # */
void MOS6502Core::OPCode0xC0() {
  OPCodesCPY(++m_PC);
  ++m_PC;
}

/* CMP X-Indirect */
void MOS6502Core::OPCode0xC1() {
  uint16_t address = m_pMemory->Read(++m_PC) + m_XR;
  address |= (m_pMemory->Read(address + 1u)) << 8u;

  OPCodesCMP(m_pMemory->Read(address));
  ++m_PC;
}

/* CPY zpg */
void MOS6502Core::OPCode0xC4() {
  OPCodesCPY(m_pMemory->Read(++m_PC));
  ++m_PC;
}

/* CMP zpg */
void MOS6502Core::OPCode0xC5() {
  OPCodesCMP(m_pMemory->Read(++m_PC));
  ++m_PC;
}

/* DEC zpg */
void MOS6502Core::OPCode0xC6() {
  OPCodesDEC(m_pMemory->Read(++m_PC));
  ++m_PC;
}

/* INY */
void MOS6502Core::OPCode0xC8() {
  ++m_YR;
  ++m_PC;

  m_YR & 0x80 ? m_SR |= NEGATIVE : m_SR &= ~NEGATIVE;
  m_YR ? m_SR &= ~ZERO : m_SR |= ZERO;
}

/* CMP # */
void MOS6502Core::OPCode0xC9() {
  OPCodesCMP(++m_PC);
  ++m_PC;
}

/* DEX */
void MOS6502Core::OPCode0xCA() {
  --m_XR;
  ++m_PC;

  m_XR & 0x80 ? m_SR |= NEGATIVE : m_SR &= ~NEGATIVE;
  m_XR == 0x00 ? m_SR |= ZERO : m_SR &= ~ZERO;
}

/* CPY abs */
void MOS6502Core::OPCode0xCC() {
  OPCodesCPY(m_pMemory->Read(m_PC + 1) | m_pMemory->Read(m_PC + 2) << 8u);
  m_PC += 3;
}

/* CMP abs */
void MOS6502Core::OPCode0xCD() {
  OPCodesCMP(m_pMemory->Read(m_PC + 1) | m_pMemory->Read(m_PC + 2) << 8u);
  m_PC += 3;
}

/* DEC abs */
void MOS6502Core::OPCode0xCE() {
  OPCodesDEC(m_pMemory->Read(m_PC + 1) | m_pMemory->Read(m_PC + 2) << 8u);
  m_PC += 3;
}

/* BNE Relative */
void MOS6502Core::OPCode0xD0() {
  m_SR & NEGATIVE ? m_PC += (int8_t)m_pMemory->Read(++m_PC) : ++m_PC;
  ++m_PC;
}

/* CMP Y-Indirect */
void MOS6502Core::OPCode0xD1() {
  uint16_t address = m_pMemory->Read(++m_PC) + m_YR;
  address |= (m_pMemory->Read(address + 1u)) << 8u;

  OPCodesCMP(m_pMemory->Read(address));
  ++m_PC;
}

/* CMP zpg, X */
void MOS6502Core::OPCode0xD5() {
  OPCodesCMP(m_pMemory->Read(++m_PC) + m_XR);
  ++m_PC;
}

/* DEC zpg, X */
void MOS6502Core::OPCode0xD6() {
  OPCodesDEC(m_pMemory->Read(++m_PC) + m_XR);
  ++m_PC;
}

/* CLD impl */
void MOS6502Core::OPCode0xD8() {
  m_SR &= ~DECIMAL;
  ++m_PC;
}

/* CMP ABS, Y */
void MOS6502Core::OPCode0xD9() {
  OPCodesCMP((m_pMemory->Read(m_PC + 1) | m_pMemory->Read(m_PC + 2) << 8u) + m_YR);
  m_PC += 3;
}

/* CMP ABS, X */
void MOS6502Core::OPCode0xDD() {
  OPCodesCMP((m_pMemory->Read(m_PC + 1) | m_pMemory->Read(m_PC + 2) << 8u) + m_XR);
  m_PC += 3;
}

/* DEC abs, X */
void MOS6502Core::OPCode0xDE() {
  OPCodesDEC((m_pMemory->Read(m_PC + 1) | m_pMemory->Read(m_PC + 2) << 8u) + m_XR);
  m_PC += 3;
}

/* CPX # */
void MOS6502Core::OPCode0xE0() {
  OPCodesCPX(++m_PC);
  ++m_PC;
}

/* SBC X-Indirect */
void MOS6502Core::OPCode0xE1() {
  uint16_t address = m_pMemory->Read(++m_PC) + m_XR;
  address |= (m_pMemory->Read(address + 1u)) << 8u;

  OPCodesSBC(m_pMemory->Read(address));
  ++m_PC;
}

/* CPX zpg */
void MOS6502Core::OPCode0xE4() {
  OPCodesCPX(m_pMemory->Read(++m_PC));
  ++m_PC;
}

/* SBC zpg */
void MOS6502Core::OPCode0xE5() {
  OPCodesSBC(m_pMemory->Read(++m_PC));
  ++m_PC;
}

/* INC zpg */
void MOS6502Core::OPCode0xE6() {
  OPCodesINC(m_pMemory->Read(++m_PC));
  ++m_PC;
}

/* INX */
void MOS6502Core::OPCode0xE8() {
  ++m_XR;
  ++m_PC;

  m_XR & 0x80 ? m_SR |= NEGATIVE : m_SR &= ~NEGATIVE;
  m_XR == 0x00 ? m_SR |= ZERO : m_SR &= ~ZERO;
}

/* SBC */
void MOS6502Core::OPCode0xE9() {
  OPCodesSBC(++m_PC);
  ++m_PC;
}

/* NOP */
void MOS6502Core::OPCode0xEA() {
  ++m_PC;
}

/* CPX, abs */
void MOS6502Core::OPCode0xEC() {
  OPCodesCPX(m_pMemory->Read(m_PC + 1) | m_pMemory->Read(m_PC + 2) << 8u);
  m_PC += 3;
}

/* SBC, abs */
void MOS6502Core::OPCode0xED() {
  OPCodesSBC(m_pMemory->Read(m_PC + 1) | m_pMemory->Read(m_PC + 2) << 8u);
  m_PC += 3;
}

/* INC, abs */
void MOS6502Core::OPCode0xEE() {
  OPCodesINC(m_pMemory->Read(m_PC + 1) | m_pMemory->Read(m_PC + 2) << 8u);
  m_PC += 3;
}

/* BEQ, Relative */
void MOS6502Core::OPCode0xF0() {
  m_SR & ZERO ? m_PC += m_pMemory->Read(++m_PC) : ++m_PC;
  ++m_PC;
}

/* SBC Indirect, Y */
void MOS6502Core::OPCode0xF1() {
  uint16_t address = m_pMemory->Read(++m_PC) + m_YR;
  address |= (m_pMemory->Read(address + 1u)) << 8u;

  OPCodesSBC(m_pMemory->Read(address));
  ++m_PC;
}

/* SBC zpg, X */
void MOS6502Core::OPCode0xF5() {
  OPCodesSBC(m_pMemory->Read(++m_PC) + m_XR);
  ++m_PC;
}

/* INC zpg, X */
void MOS6502Core::OPCode0xF6() {
  OPCodesINC(m_pMemory->Read(++m_PC) + m_XR);
  ++m_PC;
}

/* SED */
void MOS6502Core::OPCode0xF8() {
  m_SR |= DECIMAL;
  ++m_PC;
}

/* SBC abs, Y */
void MOS6502Core::OPCode0xF9() {
  OPCodesSBC((m_pMemory->Read(m_PC + 1) | m_pMemory->Read(m_PC + 2) << 8) + m_YR);
  m_PC += 3;
}

/* SBC abs, X */
void MOS6502Core::OPCode0xFD() {
  OPCodesSBC((m_pMemory->Read(m_PC + 1) | m_pMemory->Read(m_PC + 2) << 8) + m_XR);
  m_PC += 3;
}

/* INC abs, X */
void MOS6502Core::OPCode0xFE() {
  OPCodesINC((m_pMemory->Read(m_PC + 1) | m_pMemory->Read(m_PC + 2) << 8) + m_XR);
  m_PC += 3;
}

void MOS6502Core::OPCodesADC(uint16_t address) {
  uint8_t val = m_pMemory->Read(address);
  uint8_t carry = m_AC & CARRY ? 1 : 0;
  uint16_t result = m_AC + val + carry;

  if (m_SR & DECIMAL) {
    if (m_AC & 0x0F + val & 0x0F + carry > 0x09)
      result += 0x06;

    result & 0x80 ? m_SR |= NEGATIVE : m_SR &= ~NEGATIVE;
    ((m_AC ^ result) & (val ^ m_AC) & 0x80) ? m_SR |= OVERFLOW : m_SR &= ~OVERFLOW;

    if (carry = result > 0x99)
      result += 0x60;

    carry ? m_SR |= CARRY : m_SR &= ~CARRY;
  } else {
    ((m_AC ^ result) & (val ^ m_AC) & 0x80) ? m_SR |= OVERFLOW : m_SR &= ~OVERFLOW;
    result > 0xFF ? m_SR |= CARRY : m_SR &= ~CARRY;
    result & 0x80 ? m_SR |= NEGATIVE : m_SR &= ~NEGATIVE;
  }

  result & 0xFF ? m_SR &= ~ZERO : m_SR |= ZERO;

  m_AC = result & 0xFF;
}

void MOS6502Core::OPCodesSBC(uint16_t address) {
  uint8_t val = m_pMemory->Read(address);
  uint8_t carry = m_SR & CARRY ? 0 : 1;

  uint16_t result = m_AC - val - carry;
  result & 0x80u ? m_SR |= NEGATIVE : m_SR &= ~NEGATIVE;
  result ? m_SR &= ~ZERO : m_SR |= ZERO;

  ((m_AC ^ result) & (val ^ m_AC) & 0x80) ? m_SR |= OVERFLOW : m_SR &= ~OVERFLOW;

  if (m_SR & DECIMAL) {
    if (((m_AC & 0x0Fu) - carry) < (val & 0x0Fu))
      result -= 0x06;

    if (result > 0x99)
      result -= 0x60;
  }

  result < 0x100 ? m_SR |= CARRY : m_SR &= ~CARRY;
  m_AC = result & 0xFFu;
}

void MOS6502Core::OPCodesASL(uint16_t address) {
  uint8_t val = m_pMemory->Read(address);
  val & 0x80u ? m_SR |= CARRY : m_SR &= ~CARRY;

  val <<= 1u;
  m_pMemory->Write(address, val);

  val & 0x80u ? m_SR |= NEGATIVE : m_SR &=~NEGATIVE;
  val ? m_SR &= ~ZERO : m_SR |= ZERO;
}

void MOS6502Core::OPCodesCMP(uint16_t address) {
  uint16_t val = m_AC - m_pMemory->Read(address);
  val < 0x100 ? m_SR &= ~CARRY : m_SR |= CARRY;
  val & 0x80u ? m_SR |= NEGATIVE : m_SR &=~NEGATIVE;
  val ? m_SR &= ~ZERO : m_SR |= ZERO;
}

void MOS6502Core::OPCodesCPX(uint16_t address) {
  uint16_t val = m_XR - m_pMemory->Read(address);
  val < 0x100 ? m_SR &= ~CARRY : m_SR |= CARRY;
  val & 0x80u ? m_SR |= NEGATIVE : m_SR &=~NEGATIVE;
  val ? m_SR &= ~ZERO : m_SR |= ZERO;
}

void MOS6502Core::OPCodesCPY(uint16_t address) {
  uint16_t val = m_YR - m_pMemory->Read(address);
  val < 0x100 ? m_SR &= ~CARRY : m_SR |= CARRY;
  val & 0x80u ? m_SR |= NEGATIVE : m_SR &=~NEGATIVE;
  val ? m_SR &= ~ZERO : m_SR |= ZERO;
}

void MOS6502Core::OPCodesINC(uint16_t address) {
  uint8_t val = m_pMemory->Read(address);
  m_pMemory->Write(address, ++val);

  val & 0x80u ? m_SR |= NEGATIVE : m_SR &= ~NEGATIVE;
  val ? m_SR &= ~ZERO : m_SR |= ZERO;
}

void MOS6502Core::OPCodesDEC(uint16_t address) {
  uint8_t val = m_pMemory->Read(address);
  m_pMemory->Write(address, --val);

  val & 0x80u ? m_SR |= NEGATIVE : m_SR &= ~NEGATIVE;
  val ? m_SR &= ~ZERO : m_SR |= ZERO;
}

void MOS6502Core::OPCodesLDA(uint16_t address) {
  m_AC = m_pMemory->Read(address);

  m_AC & 0x80u ? m_SR |= NEGATIVE : m_SR &= ~NEGATIVE;
  m_AC ? m_SR &= ~ZERO : m_SR |= ZERO;
}

void MOS6502Core::OPCodesLDX(uint16_t address) {
  m_XR = m_pMemory->Read(address);

  m_XR & 0x80u ? m_SR |= NEGATIVE : m_SR &= ~NEGATIVE;
  m_XR ? m_SR &= ~ZERO : m_SR |= ZERO;
}

void MOS6502Core::OPCodesLDY(uint16_t address) {
  m_YR = m_pMemory->Read(address);

  m_YR & 0x80u ? m_SR |= NEGATIVE : m_SR &= ~NEGATIVE;
  m_YR ? m_SR &= ~ZERO : m_SR |= ZERO;
}

void MOS6502Core::OPCodesSTA(uint16_t address) {
  m_pMemory->Write(address, m_AC);
}

void MOS6502Core::OPCodesSTX(uint16_t address) {
  m_pMemory->Write(address, m_XR);
}

void MOS6502Core::OPCodesSTY(uint16_t address) {
  m_pMemory->Write(address, m_YR);
}

void MOS6502Core::OPCodesORA(uint16_t address) {
  m_AC |= m_pMemory->Read(address);

  m_AC & 0x80u ? m_SR |= NEGATIVE : m_SR &=~NEGATIVE;
  m_AC ? m_SR &= ~ZERO : m_SR |= ZERO;
}

void MOS6502Core::OPCodesEOR(uint16_t address) {
  m_AC ^= m_pMemory->Read(address);

  m_AC & 0x80u ? m_SR |= NEGATIVE : m_SR &=~NEGATIVE;
  m_AC ? m_SR &= ~ZERO : m_SR |= ZERO;
}

void MOS6502Core::OPCodesAND(uint16_t address) {
  m_AC &= m_pMemory->Read(address);

  m_AC & 0x80u ? m_SR |= NEGATIVE : m_SR &=~NEGATIVE;
  m_AC ? m_SR &= ~ZERO : m_SR |= ZERO;
}

void MOS6502Core::OPCodesLSR(uint16_t address) {
  uint8_t val = m_pMemory->Read(address);
  val & 0x01 ? m_SR |= CARRY : m_SR &= ~CARRY;

  val >>= 1;

  m_pMemory->Write(address, val);

  val & 0x80u ? m_SR |= NEGATIVE : m_SR &=~NEGATIVE;
  val ? m_SR &= ~ZERO : m_SR |= ZERO;
}

void MOS6502Core::OPCodesROR(uint16_t address) {
  uint16_t val = m_pMemory->Read(address);

  if (m_SR & CARRY) val |= 0x100;
  val & 0x01 ? m_SR |= CARRY : m_SR &= ~CARRY;

  val >>= 1u;

  val & 0x80 ? m_SR |= NEGATIVE : m_SR &= ~NEGATIVE;
  val ? m_SR &= ~ZERO : m_SR |= ZERO;

  m_pMemory->Write(address, val & 0xFF);
}

void MOS6502Core::OPCodesROL(uint16_t address) {
  uint16_t val = m_pMemory->Read(address) << 1u;

  if (m_SR & CARRY) val |= 0x01;
  val > 0xFF ? m_SR |= CARRY : m_SR &= ~CARRY;
  val & 0x80 ? m_SR |= NEGATIVE : m_SR &= ~NEGATIVE;
  val ? m_SR &= ~ZERO : m_SR |= ZERO;

  m_pMemory->Write(address, val & 0xFF);
}

void MOS6502Core::OPCodesBIT(uint16_t address) {
  uint8_t val = m_pMemory->Read(address) & m_AC;
  m_SR = (m_SR & 0x3F) | (val & 0xC0);

  val & 0x80u ? m_SR |= NEGATIVE : m_SR &=~NEGATIVE;
  val ? m_SR &= ~ZERO : m_SR |= ZERO;
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
