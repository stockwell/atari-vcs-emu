#include <iostream>

#include "Common.h"
#include "MOS6502Core.h"

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

void MOS6502Core::OPCode0x0A() {

}

void MOS6502Core::OPCode0x0D() {

}

void MOS6502Core::OPCode0x0E() {

}

void MOS6502Core::OPCode0x10() {

}

void MOS6502Core::OPCode0x11() {

}

void MOS6502Core::OPCode0x15() {

}

void MOS6502Core::OPCode0x16() {

}

void MOS6502Core::OPCode0x18() {

}

void MOS6502Core::OPCode0x19() {

}

void MOS6502Core::OPCode0x1D() {

}

void MOS6502Core::OPCode0x1E() {

}

void MOS6502Core::OPCode0x20() {

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

void MOS6502Core::OPCode0x30() {

}

void MOS6502Core::OPCode0x31() {

}

void MOS6502Core::OPCode0x35() {

}

void MOS6502Core::OPCode0x36() {

}

void MOS6502Core::OPCode0x38() {

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

void MOS6502Core::OPCode0x49() {

}

void MOS6502Core::OPCode0x4A() {

}

void MOS6502Core::OPCode0x4C() {

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

void MOS6502Core::OPCode0x60() {

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

void MOS6502Core::OPCode0x78() {

}

void MOS6502Core::OPCode0x79() {

}

void MOS6502Core::OPCode0x7D() {

}

void MOS6502Core::OPCode0x7E() {

}

void MOS6502Core::OPCode0x81() {

}

void MOS6502Core::OPCode0x84() {

}

void MOS6502Core::OPCode0x85() {

}

void MOS6502Core::OPCode0x86() {

}

void MOS6502Core::OPCode0x88() {

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

void MOS6502Core::OPCode0x95() {

}

void MOS6502Core::OPCode0x96() {

}

void MOS6502Core::OPCode0x98() {

}

void MOS6502Core::OPCode0x99() {

}

void MOS6502Core::OPCode0x9A() {

}

void MOS6502Core::OPCode0x9D() {

}

void MOS6502Core::OPCode0xA0() {

}

void MOS6502Core::OPCode0xA1() {

}

void MOS6502Core::OPCode0xA2() {

}

void MOS6502Core::OPCode0xA4() {

}

void MOS6502Core::OPCode0xA5() {

}

void MOS6502Core::OPCode0xA6() {

}

void MOS6502Core::OPCode0xA8() {

}

void MOS6502Core::OPCode0xA9() {

}

void MOS6502Core::OPCode0xAA() {

}

void MOS6502Core::OPCode0xAC() {

}

void MOS6502Core::OPCode0xAD() {

}

void MOS6502Core::OPCode0xAE() {

}

void MOS6502Core::OPCode0xB0() {

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

void MOS6502Core::OPCode0xC8() {

}

void MOS6502Core::OPCode0xC9() {

}

void MOS6502Core::OPCode0xCA() {

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

void MOS6502Core::OPCode0xD8() {

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

void MOS6502Core::OPCode0xE9() {

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