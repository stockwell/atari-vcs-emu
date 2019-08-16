#include <cstdio>
#include "TIACore.h"

TIACore::TIACore() {
  m_pMem = new uint8_t[0xFF];
}

TIACore::~TIACore() {
  SafeDeleteArray(m_pMem)
}

void TIACore::Init() {
  m_WriteRegisters[0x00] = &TIACore::TIAWrite0x00;
  m_WriteRegisters[0x01] = &TIACore::TIAWrite0x01;
  m_WriteRegisters[0x02] = &TIACore::TIAWrite0x02;
  m_WriteRegisters[0x03] = &TIACore::TIAWrite0x03;
  m_WriteRegisters[0x04] = &TIACore::TIAWrite0x04;
  m_WriteRegisters[0x05] = &TIACore::TIAWrite0x05;
  m_WriteRegisters[0x06] = &TIACore::TIAWrite0x06;
  m_WriteRegisters[0x07] = &TIACore::TIAWrite0x07;
  m_WriteRegisters[0x08] = &TIACore::TIAWrite0x08;
  m_WriteRegisters[0x09] = &TIACore::TIAWrite0x09;
  m_WriteRegisters[0x0A] = &TIACore::TIAWrite0x0A;
  m_WriteRegisters[0x0B] = &TIACore::TIAWrite0x0B;
  m_WriteRegisters[0x0C] = &TIACore::TIAWrite0x0C;
  m_WriteRegisters[0x0D] = &TIACore::TIAWrite0x0D;
  m_WriteRegisters[0x0E] = &TIACore::TIAWrite0x0E;
  m_WriteRegisters[0x0F] = &TIACore::TIAWrite0x0F;

  m_WriteRegisters[0x10] = &TIACore::TIAWrite0x10;
  m_WriteRegisters[0x11] = &TIACore::TIAWrite0x11;
  m_WriteRegisters[0x12] = &TIACore::TIAWrite0x12;
  m_WriteRegisters[0x13] = &TIACore::TIAWrite0x13;
  m_WriteRegisters[0x14] = &TIACore::TIAWrite0x14;
  m_WriteRegisters[0x15] = &TIACore::TIAWrite0x15;
  m_WriteRegisters[0x16] = &TIACore::TIAWrite0x16;
  m_WriteRegisters[0x17] = &TIACore::TIAWrite0x17;
  m_WriteRegisters[0x18] = &TIACore::TIAWrite0x18;
  m_WriteRegisters[0x19] = &TIACore::TIAWrite0x19;
  m_WriteRegisters[0x1A] = &TIACore::TIAWrite0x1A;
  m_WriteRegisters[0x1B] = &TIACore::TIAWrite0x1B;
  m_WriteRegisters[0x1C] = &TIACore::TIAWrite0x1C;
  m_WriteRegisters[0x1D] = &TIACore::TIAWrite0x1D;
  m_WriteRegisters[0x1E] = &TIACore::TIAWrite0x1E;
  m_WriteRegisters[0x1E] = &TIACore::TIAWrite0x1F;

  m_WriteRegisters[0x20] = &TIACore::TIAWrite0x20;
  m_WriteRegisters[0x21] = &TIACore::TIAWrite0x21;
  m_WriteRegisters[0x22] = &TIACore::TIAWrite0x22;
  m_WriteRegisters[0x23] = &TIACore::TIAWrite0x23;
  m_WriteRegisters[0x24] = &TIACore::TIAWrite0x24;
  m_WriteRegisters[0x25] = &TIACore::TIAWrite0x25;
  m_WriteRegisters[0x26] = &TIACore::TIAWrite0x26;
  m_WriteRegisters[0x27] = &TIACore::TIAWrite0x27;
  m_WriteRegisters[0x28] = &TIACore::TIAWrite0x28;
  m_WriteRegisters[0x29] = &TIACore::TIAWrite0x29;
  m_WriteRegisters[0x2A] = &TIACore::TIAWrite0x2A;
  m_WriteRegisters[0x2B] = &TIACore::TIAWrite0x2B;
  m_WriteRegisters[0x2C] = &TIACore::TIAWrite0x2C;
  m_WriteRegisters[0x2D] = &TIACore::TIAWrite0x2D;
}

uint8_t TIACore::Read(uint16_t address) {
  if (address < (sizeof kTIAReadRegisterNames / sizeof kTIAReadRegisterNames[0]))
    Log("TIA Read: %s", kTIAReadRegisterNames[address]);

  return m_pMem[address];
}

void TIACore::Write(uint16_t address, uint8_t value) {
  if (address < (sizeof kTIAWriteRegisterNames / sizeof kTIAWriteRegisterNames[0]))
    Log("TIA Write: %s (%u)", kTIAWriteRegisterNames[address], value);
}

void TIACore::TIAWrite0x00(uint8_t value){

}

void TIACore::TIAWrite0x01(uint8_t value){

}

void TIACore::TIAWrite0x02(uint8_t value){

}

void TIACore::TIAWrite0x03(uint8_t value){

}

void TIACore::TIAWrite0x04(uint8_t value){

}

void TIACore::TIAWrite0x05(uint8_t value){

}

void TIACore::TIAWrite0x06(uint8_t value){

}

void TIACore::TIAWrite0x07(uint8_t value){

}

void TIACore::TIAWrite0x08(uint8_t value){

}

void TIACore::TIAWrite0x09(uint8_t value){
  __asm("NOP");
}

void TIACore::TIAWrite0x0A(uint8_t value){

}

void TIACore::TIAWrite0x0B(uint8_t value){

}

void TIACore::TIAWrite0x0C(uint8_t value){

}

void TIACore::TIAWrite0x0D(uint8_t value){

}

void TIACore::TIAWrite0x0E(uint8_t value){

}

void TIACore::TIAWrite0x0F(uint8_t value){

}

void TIACore::TIAWrite0x10(uint8_t value){

}

void TIACore::TIAWrite0x11(uint8_t value){

}

void TIACore::TIAWrite0x12(uint8_t value){

}

void TIACore::TIAWrite0x13(uint8_t value){

}

void TIACore::TIAWrite0x14(uint8_t value){

}

void TIACore::TIAWrite0x15(uint8_t value){

}

void TIACore::TIAWrite0x16(uint8_t value){

}

void TIACore::TIAWrite0x17(uint8_t value){

}

void TIACore::TIAWrite0x18(uint8_t value){

}

void TIACore::TIAWrite0x19(uint8_t value){

}

void TIACore::TIAWrite0x1A(uint8_t value){

}

void TIACore::TIAWrite0x1B(uint8_t value){

}

void TIACore::TIAWrite0x1C(uint8_t value){

}

void TIACore::TIAWrite0x1D(uint8_t value){

}

void TIACore::TIAWrite0x1E(uint8_t value){

}

void TIACore::TIAWrite0x1F(uint8_t value){

}

void TIACore::TIAWrite0x20(uint8_t value){

}

void TIACore::TIAWrite0x21(uint8_t value){

}

void TIACore::TIAWrite0x22(uint8_t value){

}

void TIACore::TIAWrite0x23(uint8_t value){

}

void TIACore::TIAWrite0x24(uint8_t value){

}

void TIACore::TIAWrite0x25(uint8_t value){

}

void TIACore::TIAWrite0x26(uint8_t value){

}

void TIACore::TIAWrite0x27(uint8_t value){

}

void TIACore::TIAWrite0x28(uint8_t value) {

}

void TIACore::TIAWrite0x29(uint8_t value) {

}

void TIACore::TIAWrite0x2A(uint8_t value) {

}

void TIACore::TIAWrite0x2B(uint8_t value) {

}

void TIACore::TIAWrite0x2C(uint8_t value) {

}

void TIACore::TIAWrite0x2D(uint8_t value) {

}
