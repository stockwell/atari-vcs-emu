#include "MOS6502Core.h"

MOS6502Core::MOS6502Core() {

}

MOS6502Core::~MOS6502Core() {

}

void MOS6502Core::Init() {
  Reset();
}

void MOS6502Core::Reset() {
  PC = 0x0000;
  AC = 0x00;
  XR = 0x00;
  YR = 0x00;
  SR = 0x20;
  SP = 0xFF;
}

uint8_t MOS6502Core::FetchOPCode() {

}

void MOS6502Core::ExecuteOPCode(uint8_t opcode) {

}

uint8_t MOS6502Core::Tick() {
  ExecuteOPCode(FetchOPCode());
}