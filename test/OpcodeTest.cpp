/****************************************************************************
* Unit Test Includes
*****************************************************************************/
#include "gtest/gtest.h"

#include "MOS6502Core.h"
#include "Memory.h"

/*****************************************************************************
* Unit Tests
*****************************************************************************/

class MOS6502Test : public ::testing::Test {
protected:
    void SetUp() override {
      m_pMemory = new Memory();
      m_pMemory->Reset();

      m_pProcessor = new MOS6502Core(m_pMemory);
      m_pProcessor->Reset();
      m_pMemory->Load(0xfffd, 0xf0);
      m_pProcessor->m_PC = 0xf000;
    }

    void TearDown() override {
      delete(m_pMemory);
      delete(m_pProcessor);
    }

    Memory *m_pMemory;
    MOS6502Core *m_pProcessor;
};

TEST_F(MOS6502Test, ResetTest) {
  m_pMemory->Load(0xfffd, 0xAA);
  m_pMemory->Load(0xfffc, 0x55);

  m_pProcessor->Reset();

  ASSERT_EQ(m_pProcessor->m_SP, 0xFD);
  ASSERT_EQ(m_pProcessor->m_PC, 0xAA55);
}

/* 0x00 */
TEST_F(MOS6502Test, OpcodeBRK) {

}

/* 0x01 */
TEST_F(MOS6502Test, OpcodeORA_IND_X) {
  uint8_t instr[] = {0x01, 0x7F};  /* ORA $7F, X */

  m_pMemory->Load(0xf000, instr, sizeof instr);

  /* Effective address is in addresses 0x80 & 0x81 -> 0x85 */
  m_pMemory->Write(0x80, 0x85);
  m_pMemory->Write(0x81, 0x00);
  m_pMemory->Write(0x85, 0x40);

  m_pProcessor->m_XR = 0x01;
  m_pProcessor->m_AC = 0x20;

  m_pProcessor->Tick();

  ASSERT_EQ(0x60, m_pProcessor->m_AC);
  ASSERT_EQ(0x00, m_pProcessor->m_SR & NEGATIVE);
  ASSERT_EQ(0x00, m_pProcessor->m_SR & ZERO);
  ASSERT_EQ(0xf002, m_pProcessor->m_PC);
}

/* 0x05 */
TEST_F(MOS6502Test, OpcodeORA_ZPG) {
  uint8_t instr[] = {0x05, 0x80};  /* ORA $80 */

  m_pMemory->Load(0xf000, instr, sizeof instr);

  m_pMemory->Write(0x80, 0x40);

  m_pProcessor->m_AC = 0x20;

  m_pProcessor->Tick();

  ASSERT_EQ(0x60, m_pProcessor->m_AC);
  ASSERT_EQ(0x00, m_pProcessor->m_SR & NEGATIVE);
  ASSERT_EQ(0x00, m_pProcessor->m_SR & ZERO);
  ASSERT_EQ(0xf002, m_pProcessor->m_PC);
}

/* 0x06 */
TEST_F(MOS6502Test, OpcodeASL_ZPG) {
  uint8_t instr[] = {0x06, 0x80,  /* ASL $80 */
                     0x06, 0x80,  /* ASL $80 */
                     0x06, 0x80,  /* ASL $80 */
                     0x06, 0x80}; /* ASL $80 */

  m_pMemory->Load(0xf000, instr, sizeof instr);
  m_pMemory->Write(0x80, 0x20);
  m_pProcessor->Tick();

  ASSERT_EQ(0x40, m_pMemory->Read(0x80));
  ASSERT_EQ(0x00, m_pProcessor->m_SR & NEGATIVE);
  ASSERT_EQ(0x00, m_pProcessor->m_SR & CARRY);
  ASSERT_EQ(0x00, m_pProcessor->m_SR & ZERO);

  m_pProcessor->Tick();

  ASSERT_EQ(0x80, m_pMemory->Read(0x80));
  ASSERT_EQ(NEGATIVE, m_pProcessor->m_SR & NEGATIVE);
  ASSERT_EQ(0x00, m_pProcessor->m_SR & CARRY);
  ASSERT_EQ(0x00, m_pProcessor->m_SR & ZERO);

  m_pProcessor->Tick();

  ASSERT_EQ(0x00, m_pMemory->Read(0x80));
  ASSERT_EQ(0x00, m_pProcessor->m_SR & NEGATIVE);
  ASSERT_EQ(CARRY, m_pProcessor->m_SR & CARRY);
  ASSERT_EQ(ZERO, m_pProcessor->m_SR & ZERO);

  m_pProcessor->Tick();

  ASSERT_EQ(0x00, m_pMemory->Read(0x80));
  ASSERT_EQ(0x00, m_pProcessor->m_SR & NEGATIVE);
  ASSERT_EQ(0x00, m_pProcessor->m_SR & CARRY);
  ASSERT_EQ(ZERO, m_pProcessor->m_SR & ZERO);

  ASSERT_EQ(0xF008, m_pProcessor->m_PC);
}

/* 0x08 */
TEST_F(MOS6502Test, OpcodePHP) {
  m_pMemory->Load(0xF000, 0x08); /* PHA */
  m_pProcessor->m_SR = 0xFF;

  m_pProcessor->Tick();

  ASSERT_EQ(0xFC, m_pProcessor->m_SP);
  ASSERT_EQ(0xFF, m_pMemory->Read(STACK_BASE + 0xFD));
  ASSERT_EQ(0xF001, m_pProcessor->m_PC);
}

/* 0x09 */
TEST_F(MOS6502Test, OpcodeORA_IM) {
  uint8_t instr[] = {0x09, 0x40};  /* ORA #$40 */

  m_pMemory->Load(0xf000, instr, sizeof instr);

  m_pProcessor->m_AC = 0x20;

  m_pProcessor->Tick();

  ASSERT_EQ(0x60, m_pProcessor->m_AC);
  ASSERT_EQ(0x00, m_pProcessor->m_SR & NEGATIVE);
  ASSERT_EQ(0x00, m_pProcessor->m_SR & ZERO);
  ASSERT_EQ(0xf002, m_pProcessor->m_PC);
}

/* 0x0A */
TEST_F(MOS6502Test, OpcodeASL_A) {
  uint8_t instr[] = {0x0A,  /* ASL, A */
                     0x0A,  /* ASL, A */
                     0x0A,  /* ASL, A */
                     0x0A}; /* ASL, A */

  m_pMemory->Load(0xf000, instr, sizeof instr);

  m_pProcessor->m_AC = 0x20;
  m_pProcessor->Tick();

  ASSERT_EQ(0x40, m_pProcessor->m_AC);
  ASSERT_EQ(0x00, m_pProcessor->m_SR & NEGATIVE);
  ASSERT_EQ(0x00, m_pProcessor->m_SR & CARRY);
  ASSERT_EQ(0x00, m_pProcessor->m_SR & ZERO);

  m_pProcessor->Tick();

  ASSERT_EQ(0x80, m_pProcessor->m_AC);
  ASSERT_EQ(NEGATIVE, m_pProcessor->m_SR & NEGATIVE);
  ASSERT_EQ(0x00, m_pProcessor->m_SR & CARRY);
  ASSERT_EQ(0x00, m_pProcessor->m_SR & ZERO);

  m_pProcessor->Tick();

  ASSERT_EQ(0x00, m_pProcessor->m_AC);
  ASSERT_EQ(0x00, m_pProcessor->m_SR & NEGATIVE);
  ASSERT_EQ(CARRY, m_pProcessor->m_SR & CARRY);
  ASSERT_EQ(ZERO, m_pProcessor->m_SR & ZERO);

  m_pProcessor->Tick();

  ASSERT_EQ(0x00, m_pProcessor->m_AC);
  ASSERT_EQ(0x00, m_pProcessor->m_SR & NEGATIVE);
  ASSERT_EQ(0x00, m_pProcessor->m_SR & CARRY);
  ASSERT_EQ(ZERO, m_pProcessor->m_SR & ZERO);

  ASSERT_EQ(0xF004, m_pProcessor->m_PC);
}

/* 0x0D */
TEST_F(MOS6502Test, OpcodeORA_ABS) {

}

/* 0x0E */
TEST_F(MOS6502Test, OpcodeASL_ABS) {
  uint8_t instr[] = {0x0E, 0x80, 0x00,   /* ASL $80 */
                     0x0E, 0x80, 0x00,   /* ASL $80 */
                     0x0E, 0x80, 0x00,   /* ASL $80 */
                     0x0E, 0x80, 0x00 }; /* ASL $80 */

  m_pMemory->Load(0xf000, instr, sizeof instr);
  m_pMemory->Load(0x80, 0x20);
  m_pProcessor->Tick();

  ASSERT_EQ(0x40, m_pMemory->Read(0x80));
  ASSERT_EQ(0x00, m_pProcessor->m_SR & NEGATIVE);
  ASSERT_EQ(0x00, m_pProcessor->m_SR & CARRY);
  ASSERT_EQ(0x00, m_pProcessor->m_SR & ZERO);

  m_pProcessor->Tick();

  ASSERT_EQ(0x80, m_pMemory->Read(0x80));
  ASSERT_EQ(NEGATIVE, m_pProcessor->m_SR & NEGATIVE);
  ASSERT_EQ(0x00, m_pProcessor->m_SR & CARRY);
  ASSERT_EQ(0x00, m_pProcessor->m_SR & ZERO);

  m_pProcessor->Tick();

  ASSERT_EQ(0x00, m_pMemory->Read(0x80));
  ASSERT_EQ(0x00, m_pProcessor->m_SR & NEGATIVE);
  ASSERT_EQ(CARRY, m_pProcessor->m_SR & CARRY);
  ASSERT_EQ(ZERO, m_pProcessor->m_SR & ZERO);

  m_pProcessor->Tick();

  ASSERT_EQ(0x00, m_pMemory->Read(0x80));
  ASSERT_EQ(0x00, m_pProcessor->m_SR & NEGATIVE);
  ASSERT_EQ(0x00, m_pProcessor->m_SR & CARRY);
  ASSERT_EQ(ZERO, m_pProcessor->m_SR & ZERO);

  ASSERT_EQ(0xF00C, m_pProcessor->m_PC);
}

/* 0x10 */
TEST_F(MOS6502Test, OpcodeBPL) {
  uint8_t instr[] = {0x10, 0xFB,  /* BPL -5*/
                     0x10, 0xFB,  /* BPL -5 */
                     0x10, 0x05,  /* BPL 5 */
                     0x10, 0x05}; /* BPL 5 */

  m_pMemory->Load(0xf000, instr, sizeof instr);

  m_pProcessor->m_SR |= NEGATIVE;

  m_pProcessor->Tick();
  ASSERT_EQ(0xF002, m_pProcessor->m_PC);

  m_pProcessor->m_SR &= ~NEGATIVE;
  m_pProcessor->Tick();
  ASSERT_EQ(0xEFFF, m_pProcessor->m_PC);

  m_pProcessor->m_PC = 0xF004;
  m_pProcessor->Tick();
  ASSERT_EQ(0xF00B, m_pProcessor->m_PC);

  m_pProcessor->m_SR |= NEGATIVE;
  m_pProcessor->m_PC = 0xF006;
  m_pProcessor->Tick();
  ASSERT_EQ(0xF008, m_pProcessor->m_PC);
}

/* 0x11 */
TEST_F(MOS6502Test, OPcodeORA_IND_Y) {
  uint8_t instr[] = {0x11, 0x7F};  /* ORA $7F, X */

  m_pMemory->Load(0xf000, instr, sizeof instr);

  /* Effective address is in addresses 0x80 & 0x81 -> 0x85 */
  m_pMemory->Write(0x80, 0x85);
  m_pMemory->Write(0x81, 0x00);
  m_pMemory->Write(0x85, 0x40);

  m_pProcessor->m_YR = 0x01;
  m_pProcessor->m_AC = 0x20;

  m_pProcessor->Tick();

  ASSERT_EQ(0x60, m_pProcessor->m_AC);
  ASSERT_EQ(0x00, m_pProcessor->m_SR & NEGATIVE);
  ASSERT_EQ(0x00, m_pProcessor->m_SR & ZERO);
  ASSERT_EQ(0xf002, m_pProcessor->m_PC);
}

/* 0x15 */
TEST_F(MOS6502Test, OPcodeORA_ZPG_X) {
  uint8_t instr[] = {0x15, 0x7F};  /* ORA $80, X */

  m_pMemory->Load(0xf000, instr, sizeof instr);

  m_pMemory->Write(0x80, 0x40);

  m_pProcessor->m_XR = 0x01;
  m_pProcessor->m_AC = 0x20;

  m_pProcessor->Tick();

  ASSERT_EQ(0x60, m_pProcessor->m_AC);
  ASSERT_EQ(0x00, m_pProcessor->m_SR & NEGATIVE);
  ASSERT_EQ(0x00, m_pProcessor->m_SR & ZERO);
  ASSERT_EQ(0xf002, m_pProcessor->m_PC);
}

/* 0x16 */
TEST_F(MOS6502Test, OPcodeASL_ZPG_X) {
  uint8_t instr[] = {0x16, 0x80,   /* ASL $80, X */
                     0x16, 0x80,   /* ASL $80, X */
                     0x16, 0x80,   /* ASL $80, X */
                     0x16, 0x80 }; /* ASL $80, X */

  m_pMemory->Load(0xf000, instr, sizeof instr);
  m_pMemory->Load(0x81, 0x20);
  m_pProcessor->m_XR = 0x01;

  m_pProcessor->Tick();

  ASSERT_EQ(0x40, m_pMemory->Read(0x81));
  ASSERT_EQ(0x00, m_pProcessor->m_SR & NEGATIVE);
  ASSERT_EQ(0x00, m_pProcessor->m_SR & CARRY);
  ASSERT_EQ(0x00, m_pProcessor->m_SR & ZERO);

  m_pProcessor->Tick();

  ASSERT_EQ(0x80, m_pMemory->Read(0x81));
  ASSERT_EQ(NEGATIVE, m_pProcessor->m_SR & NEGATIVE);
  ASSERT_EQ(0x00, m_pProcessor->m_SR & CARRY);
  ASSERT_EQ(0x00, m_pProcessor->m_SR & ZERO);

  m_pProcessor->Tick();

  ASSERT_EQ(0x00, m_pMemory->Read(0x81));
  ASSERT_EQ(0x00, m_pProcessor->m_SR & NEGATIVE);
  ASSERT_EQ(CARRY, m_pProcessor->m_SR & CARRY);
  ASSERT_EQ(ZERO, m_pProcessor->m_SR & ZERO);

  m_pProcessor->Tick();

  ASSERT_EQ(0x00, m_pMemory->Read(0x81));
  ASSERT_EQ(0x00, m_pProcessor->m_SR & NEGATIVE);
  ASSERT_EQ(0x00, m_pProcessor->m_SR & CARRY);
  ASSERT_EQ(ZERO, m_pProcessor->m_SR & ZERO);

  ASSERT_EQ(0xF008, m_pProcessor->m_PC);
}

/* 0x18 */
TEST_F(MOS6502Test, OPCodeCLC) {
  m_pMemory->Load(0xf000, 0x18); /* CLC */

  m_pProcessor->m_SR |= CARRY;
  m_pProcessor->Tick();

  ASSERT_EQ(0x00, m_pProcessor->m_SR & CARRY);
  ASSERT_EQ(0xF001, m_pProcessor->m_PC);
}

/* 0x19 */
TEST_F(MOS6502Test, OPcodeORA_ABS_Y) {

}

/* 0x1D */
TEST_F(MOS6502Test, OPcodeORA_ABS_X) {

}

/* 0x1E */
TEST_F(MOS6502Test, OPcodeASL_ABS_X) {
  uint8_t instr[] = {0x1E, 0x80, 0x00,   /* ASL $80, X */
                     0x1E, 0x80, 0x00,   /* ASL $80, X */
                     0x1E, 0x80, 0x00,   /* ASL $80, X */
                     0x1E, 0x80, 0x00 }; /* ASL $80, X */

  m_pMemory->Load(0xf000, instr, sizeof instr);
  m_pMemory->Load(0x81, 0x20);
  m_pProcessor->m_XR = 0x01;

  m_pProcessor->Tick();

  ASSERT_EQ(0x40, m_pMemory->Read(0x81));
  ASSERT_EQ(0x00, m_pProcessor->m_SR & NEGATIVE);
  ASSERT_EQ(0x00, m_pProcessor->m_SR & CARRY);
  ASSERT_EQ(0x00, m_pProcessor->m_SR & ZERO);

  m_pProcessor->Tick();

  ASSERT_EQ(0x80, m_pMemory->Read(0x81));
  ASSERT_EQ(NEGATIVE, m_pProcessor->m_SR & NEGATIVE);
  ASSERT_EQ(0x00, m_pProcessor->m_SR & CARRY);
  ASSERT_EQ(0x00, m_pProcessor->m_SR & ZERO);

  m_pProcessor->Tick();

  ASSERT_EQ(0x00, m_pMemory->Read(0x81));
  ASSERT_EQ(0x00, m_pProcessor->m_SR & NEGATIVE);
  ASSERT_EQ(CARRY, m_pProcessor->m_SR & CARRY);
  ASSERT_EQ(ZERO, m_pProcessor->m_SR & ZERO);

  m_pProcessor->Tick();

  ASSERT_EQ(0x00, m_pMemory->Read(0x81));
  ASSERT_EQ(0x00, m_pProcessor->m_SR & NEGATIVE);
  ASSERT_EQ(0x00, m_pProcessor->m_SR & CARRY);
  ASSERT_EQ(ZERO, m_pProcessor->m_SR & ZERO);

  ASSERT_EQ(0xF00C, m_pProcessor->m_PC);
}

/* 0x20 */
TEST_F(MOS6502Test, OPcodeJSR) {
  uint8_t instr[] = {0x20, 0x80, 0x00};   /* JSR $80 */
  m_pMemory->Load(0xf000, instr, sizeof instr);

  m_pProcessor->Tick();

  ASSERT_EQ(0xFB, m_pProcessor->m_SP);
  ASSERT_EQ(0x80, m_pProcessor->m_PC);
  ASSERT_EQ(0x03, m_pMemory->Read(STACK_BASE + 0xFC));
  ASSERT_EQ(0xF0, m_pMemory->Read(STACK_BASE + 0xFD));

}

/* 0x21 */
TEST_F(MOS6502Test, OPcodeAND_X_IND) {

}

/* 0x24 */
TEST_F(MOS6502Test, OPcodeBIT_ZPG) {

}

/* 0x25 */
TEST_F(MOS6502Test, OPcodeAND_ZPG) {

}

/* 0x26 */
TEST_F(MOS6502Test, OPcodeROL_ZPG) {

}

/* 0x28 */
TEST_F(MOS6502Test, OPcodePLP) {
  uint8_t instr[] = {0x48,  /* PHA */
                     0x28}; /* PLP */

  m_pMemory->Load(0xf000, instr, sizeof instr);
  m_pProcessor->m_AC = 0x55;
  m_pProcessor->m_SR = 0x00;

  m_pProcessor->Tick();
  m_pProcessor->Tick();

  ASSERT_EQ(0x55 | CONSTANT, m_pProcessor->m_SR);
  ASSERT_EQ(0xF002, m_pProcessor->m_PC);
}

/* 0x30 */
TEST_F(MOS6502Test, OPcodeBMI) {
  uint8_t instr[] = {0x30, 0xFB,  /* BMI -5*/
                     0x30, 0xFB,  /* BMI -5 */
                     0x30, 0x05,  /* BMI 5 */
                     0x30, 0x05}; /* BMI 5 */

  m_pMemory->Load(0xf000, instr, sizeof instr);

  m_pProcessor->m_SR &= ~NEGATIVE;

  m_pProcessor->Tick();
  ASSERT_EQ(0xF002, m_pProcessor->m_PC);

  m_pProcessor->m_SR |= NEGATIVE;
  m_pProcessor->Tick();
  ASSERT_EQ(0xEFFF, m_pProcessor->m_PC);

  m_pProcessor->m_PC = 0xF004;
  m_pProcessor->Tick();
  ASSERT_EQ(0xF00B, m_pProcessor->m_PC);

  m_pProcessor->m_SR &= ~NEGATIVE;
  m_pProcessor->m_PC = 0xF006;
  m_pProcessor->Tick();
  ASSERT_EQ(0xF008, m_pProcessor->m_PC);
}

/* 0x38 */
TEST_F(MOS6502Test, OpcodeSEC) {
  m_pMemory->Load(0xf000, 0x38); /* SEC */

  m_pProcessor->Tick();

  ASSERT_EQ(CARRY, m_pProcessor->m_SR & CARRY);

  ASSERT_EQ(0xF001, m_pProcessor->m_PC);
}

/* 0x40 */
TEST_F(MOS6502Test, OpcodeRTI) {
  m_pMemory->Load(0xf000, 0x40); /* RTI */

  m_pMemory->Write(STACK_BASE + 0xF0, 0xAA);
  m_pMemory->Write(STACK_BASE + 0xF1, 0x00);
  m_pMemory->Write(STACK_BASE + 0xF2, 0xF0);

  m_pProcessor->m_SR = 0x55;
  m_pProcessor->m_SP = 0xEF;

  m_pProcessor->Tick();

  ASSERT_EQ(0xAA | CONSTANT, m_pProcessor->m_SR);
  ASSERT_EQ(0xF000, m_pProcessor->m_PC);
}

/* 0x48 */
TEST_F(MOS6502Test, OPCodePHA) {
  m_pMemory->Load(0xF000, 0x48); /* PHA */
  m_pProcessor->m_AC = 0x80;

  m_pProcessor->Tick();

  ASSERT_EQ(0xFC, m_pProcessor->m_SP);
  ASSERT_EQ(0x80, m_pMemory->Read(STACK_BASE + 0xFD));
  ASSERT_EQ(0xF001, m_pProcessor->m_PC);
}

/* 0x49 */
TEST_F(MOS6502Test, OPCodeEOR) {

}

/* 0x4C */
TEST_F(MOS6502Test, OPcodeJMP_ABS) {
  uint8_t instr[] = {0x4C, 0x80, 0xF0};   /* JMP $f080 */
  m_pMemory->Load(0xf000, instr, sizeof instr);

  m_pProcessor->Tick();

  ASSERT_EQ(0xF080, m_pProcessor->m_PC);
}

/* 0x50 */
TEST_F(MOS6502Test, OPcodeBVC) {
  uint8_t instr[] = {0x50, 0xFB,  /* BVC -5*/
                     0x50, 0xFB,  /* BVC -5 */
                     0x50, 0x05,  /* BVC 5 */
                     0x50, 0x05}; /* BVC 5 */

  m_pMemory->Load(0xf000, instr, sizeof instr);

  m_pProcessor->m_SR |= OVERFLOW;

  m_pProcessor->Tick();
  ASSERT_EQ(0xF002, m_pProcessor->m_PC);

  m_pProcessor->m_SR &= ~OVERFLOW;
  m_pProcessor->Tick();
  ASSERT_EQ(0xEFFF, m_pProcessor->m_PC);

  m_pProcessor->m_PC = 0xF004;
  m_pProcessor->Tick();
  ASSERT_EQ(0xF00B, m_pProcessor->m_PC);

  m_pProcessor->m_SR |= OVERFLOW;
  m_pProcessor->m_PC = 0xF006;
  m_pProcessor->Tick();
  ASSERT_EQ(0xF008, m_pProcessor->m_PC);
}

/* 0x58 */
TEST_F(MOS6502Test, OpcodeCLI) {
  m_pMemory->Load(0xf000, 0x58); /* CLI */

  m_pProcessor->m_SR |= INTERRUPT;
  m_pProcessor->Tick();

  ASSERT_EQ(0x00, m_pProcessor->m_SR & INTERRUPT);
  ASSERT_EQ(0xF001, m_pProcessor->m_PC);
}

/* 0x68 */
TEST_F(MOS6502Test, OPcodePLA) {
  uint8_t instr[] = {0x08,  /* PHP */
                     0x08,  /* PHP */
                     0x68,  /* PLA */
                     0x68}; /* PLA */

  m_pMemory->Load(0xf000, instr, sizeof instr);
  m_pProcessor->m_AC = 0xAA;

  m_pProcessor->m_SR = 0xF0;
  m_pProcessor->Tick();

  m_pProcessor->m_SR = 0x00;
  m_pProcessor->Tick();

  m_pProcessor->Tick();
  ASSERT_EQ(0x00, m_pProcessor->m_AC);
  ASSERT_EQ(0x00, m_pProcessor->m_SR & NEGATIVE);
  ASSERT_EQ(ZERO, m_pProcessor->m_SR & ZERO);

  m_pProcessor->Tick();
  ASSERT_EQ(0xF0, m_pProcessor->m_AC);
  ASSERT_EQ(NEGATIVE, m_pProcessor->m_SR & NEGATIVE);
  ASSERT_EQ(0x00, m_pProcessor->m_SR & ZERO);

  ASSERT_EQ(0xF004, m_pProcessor->m_PC);
}

/* 0x69 */
TEST_F(MOS6502Test, OPcodeADC) {

}

/* 0x6C */
TEST_F(MOS6502Test, OPcodeJMP_Indirect) {
  uint8_t instr[] = {0x6C, 0x80, 0x00};  /* JMP ($0080)*/

  m_pMemory->Load(0xF000, instr, sizeof instr);
  m_pMemory->Load(0x80, 0xFF);

  m_pProcessor->Tick();

  ASSERT_EQ(0xFF, m_pProcessor->m_PC);
}

/* 0x70 */
TEST_F(MOS6502Test, OPcodeBVS) {
  uint8_t instr[] = {0x70, 0xFB,  /* BVS -5*/
                     0x70, 0xFB,  /* BVS -5 */
                     0x70, 0x05,  /* BVS 5 */
                     0x70, 0x05}; /* BVS 5 */

  m_pMemory->Load(0xf000, instr, sizeof instr);

  m_pProcessor->m_SR &= ~OVERFLOW;

  m_pProcessor->Tick();
  ASSERT_EQ(0xF002, m_pProcessor->m_PC);

  m_pProcessor->m_SR |= OVERFLOW;
  m_pProcessor->Tick();
  ASSERT_EQ(0xEFFF, m_pProcessor->m_PC);

  m_pProcessor->m_PC = 0xF004;
  m_pProcessor->Tick();
  ASSERT_EQ(0xF00B, m_pProcessor->m_PC);

  m_pProcessor->m_SR &= ~OVERFLOW;
  m_pProcessor->m_PC = 0xF006;
  m_pProcessor->Tick();
  ASSERT_EQ(0xF008, m_pProcessor->m_PC);
}

/* 0x78 */
TEST_F(MOS6502Test, OpcodeSEI) {
  m_pMemory->Load(0xf000, 0x78); /* SEI */

  m_pProcessor->Tick();

  ASSERT_EQ(INTERRUPT, m_pProcessor->m_SR & INTERRUPT);

  ASSERT_EQ(0xF001, m_pProcessor->m_PC);
}

/* 0x84 */
TEST_F(MOS6502Test, OpcodeSTY_ZPG) {
  uint8_t instr[] = {0x84, 0x80};  /* STY $80*/
  m_pMemory->Load(0xf000, instr, sizeof instr);
  m_pProcessor->m_YR = 0x80;

  m_pProcessor->Tick();

  ASSERT_EQ(0x80, m_pMemory->Read(0x80));

  ASSERT_EQ(0xF002, m_pProcessor->m_PC);
}

/* 0x85 */
TEST_F(MOS6502Test, OpcodeSTA_ZPG) {
  uint8_t instr[] = {0x85, 0x80};   /* STA $80 */
  m_pMemory->Load(0xf000, instr, sizeof instr);

  m_pProcessor->m_AC = 0x55;
  m_pProcessor->Tick();

  ASSERT_EQ(0x55, m_pMemory->Read(0x80));
  ASSERT_EQ(0xF002, m_pProcessor->m_PC);
}

/* 0x86 */
TEST_F(MOS6502Test, OpcodeSTX_ZPG) {
  uint8_t instr[] = {0x86, 0x80};   /* STX $80 */
  m_pMemory->Load(0xf000, instr, sizeof instr);

  m_pProcessor->m_XR = 0x55;
  m_pProcessor->Tick();

  ASSERT_EQ(0x55, m_pMemory->Read(0x80));
  ASSERT_EQ(0xF002, m_pProcessor->m_PC);
}

/* 0x88 */
TEST_F(MOS6502Test, DEY) {
  uint8_t instr[] = {0x88,   /* DEY */
                     0x88,   /* DEY */
                     0x88};  /* DEY */
  m_pMemory->Load(0xf000, instr, sizeof instr);
  m_pProcessor->m_YR = 0x02;

  m_pProcessor->Tick();

  ASSERT_EQ(0x01, m_pProcessor->m_YR);
  ASSERT_EQ(0x00, m_pProcessor->m_SR & NEGATIVE);
  ASSERT_EQ(0x00, m_pProcessor->m_SR & ZERO);

  m_pProcessor->Tick();

  ASSERT_EQ(0x00, m_pProcessor->m_YR);
  ASSERT_EQ(0x00, m_pProcessor->m_SR & NEGATIVE);
  ASSERT_EQ(ZERO, m_pProcessor->m_SR & ZERO);

  m_pProcessor->Tick();

  ASSERT_EQ(0xFF, m_pProcessor->m_YR);
  ASSERT_EQ(NEGATIVE, m_pProcessor->m_SR & NEGATIVE);
  ASSERT_EQ(0x00, m_pProcessor->m_SR & ZERO);

  ASSERT_EQ(0xF003, m_pProcessor->m_PC);
}

/* 0x8A */
TEST_F(MOS6502Test, OpcodeTXA) {
  uint8_t instr[] = {0x8A};   /* TXA */
  m_pMemory->Load(0xf000, instr, sizeof instr);

  m_pProcessor->m_XR = 0x55;
  m_pProcessor->Tick();

  ASSERT_EQ(0x55, m_pProcessor->m_AC);

  ASSERT_EQ(0xF001, m_pProcessor->m_PC);
}

/* 0x8C */
TEST_F(MOS6502Test, OpcodeSTY_ABS) {
  uint8_t instr[] = {0x8C, 0x80, 0x00};  /* STY $80*/
  m_pMemory->Load(0xf000, instr, sizeof instr);
  m_pProcessor->m_YR = 0x80;

  m_pProcessor->Tick();

  ASSERT_EQ(0x80, m_pMemory->Read(0x80));

  ASSERT_EQ(0xF003, m_pProcessor->m_PC);
}

/* 0x8D */
TEST_F(MOS6502Test, OpcodeSTA_ABS) {
  uint8_t instr[] = {0x8D, 0x80, 0x00};  /* STA $80*/
  m_pMemory->Load(0xf000, instr, sizeof instr);
  m_pProcessor->m_AC = 0x80;

  m_pProcessor->Tick();

  ASSERT_EQ(0x80, m_pMemory->Read(0x80));

  ASSERT_EQ(0xF003, m_pProcessor->m_PC);
}

/* 0x8E */
TEST_F(MOS6502Test, OpcodeSTX_ABS) {
  uint8_t instr[] = {0x8E, 0x80, 0x00};  /* STX $80*/
  m_pMemory->Load(0xf000, instr, sizeof instr);
  m_pProcessor->m_XR = 0x80;

  m_pProcessor->Tick();

  ASSERT_EQ(0x80, m_pMemory->Read(0x80));

  ASSERT_EQ(0xF003, m_pProcessor->m_PC);
}

/* 0x90 */
TEST_F(MOS6502Test, OPcodeBCC) {
  uint8_t instr[] = {0x90, 0xFB,  /* BCC -5*/
                     0x90, 0xFB,  /* BCC -5 */
                     0x90, 0x05,  /* BCC 5 */
                     0x90, 0x05}; /* BCC 5 */

  m_pMemory->Load(0xf000, instr, sizeof instr);

  m_pProcessor->m_SR |= CARRY;

  m_pProcessor->Tick();
  ASSERT_EQ(0xF002, m_pProcessor->m_PC);

  m_pProcessor->m_SR &= ~CARRY;
  m_pProcessor->Tick();
  ASSERT_EQ(0xEFFF, m_pProcessor->m_PC);

  m_pProcessor->m_PC = 0xF004;
  m_pProcessor->Tick();
  ASSERT_EQ(0xF00B, m_pProcessor->m_PC);

  m_pProcessor->m_SR |= CARRY;
  m_pProcessor->m_PC = 0xF006;
  m_pProcessor->Tick();
  ASSERT_EQ(0xF008, m_pProcessor->m_PC);
}

/* 0x95 */
TEST_F(MOS6502Test, OpcodeSTA_ZPG_X) {
  uint8_t instr[] = {0x95, 0x80};   /* STA $80, X */
  m_pMemory->Load(0xf000, instr, sizeof instr);

  m_pProcessor->m_AC = 0x55;
  m_pProcessor->m_XR = 1;
  m_pProcessor->Tick();

  ASSERT_EQ(0x55, m_pMemory->Read(0x81));

  ASSERT_EQ(0xF002, m_pProcessor->m_PC);
}


/* 0x98 */
TEST_F(MOS6502Test, OpcodeTYA) {
  uint8_t instr[] = {0x98};   /* TYA */
  m_pMemory->Load(0xf000, instr, sizeof instr);

  m_pProcessor->m_YR = 0x55;
  m_pProcessor->Tick();

  ASSERT_EQ(0x55, m_pProcessor->m_AC);

  ASSERT_EQ(0xF001, m_pProcessor->m_PC);
}

/* 0x99 */
TEST_F(MOS6502Test, OpcodeSTA_ABS_Y) {
  uint8_t instr[] = {0x99, 0x80, 0x00,  /* STA $0080,Y */
                     0x99, 0x80, 0x00,  /* STA $0080,Y */
                     0x99, 0x80, 0x00}; /* STA $0080,Y */

  m_pMemory->Load(0xf000, instr, sizeof instr);
  m_pMemory->Load(0x81, 0xAA);


  m_pProcessor->m_YR = 0x01;
  m_pProcessor->m_AC = 0x55;
  m_pProcessor->Tick();

  ASSERT_EQ(0x55, m_pMemory->Read(0x81));
  ASSERT_EQ(0xF003, m_pProcessor->m_PC);
}


/* 0x9A */
TEST_F(MOS6502Test, OpcodeTXS) {
  uint8_t instr[] = {0x9A};   /* TXS */
  m_pMemory->Load(0xf000, instr, sizeof instr);

  m_pProcessor->m_XR = 0x55;
  m_pProcessor->Tick();

  ASSERT_EQ(0x55, m_pProcessor->m_SP);

  ASSERT_EQ(0xF001, m_pProcessor->m_PC);
}

/* 0x9D */
TEST_F(MOS6502Test, OpcodeSTA_ABS_X) {
  uint8_t instr[] = {0x9D, 0x80, 0x00,  /* STA $0080,X */
                     0x9D, 0x80, 0x00,  /* STA $0080,X */
                     0x9D, 0x80, 0x00}; /* STA $0080,X */

  m_pMemory->Load(0xf000, instr, sizeof instr);
  m_pMemory->Load(0x81, 0xAA);

  m_pProcessor->m_XR = 0x01;
  m_pProcessor->m_AC = 0x55;
  m_pProcessor->Tick();

  ASSERT_EQ(0x55, m_pMemory->Read(0x81));
  ASSERT_EQ(0xF003, m_pProcessor->m_PC);
}

/* 0xA0 */
TEST_F(MOS6502Test, OpcodeLDY_Immediate) {
  uint8_t instr[] = {0xA0, 0x02,  /* LDY #02 */
                     0xA0, 0x00,  /* LDY #00 */
                     0xA0, 0x85}; /* LDY #85 */
  m_pMemory->Load(0xf000, instr, sizeof instr);

  m_pProcessor->Tick();

  ASSERT_EQ(0x02, m_pProcessor->m_YR);
  ASSERT_EQ(0x00, m_pProcessor->m_SR & NEGATIVE);
  ASSERT_EQ(0x00, m_pProcessor->m_SR & ZERO);

  m_pProcessor->Tick();

  ASSERT_EQ(0x00, m_pProcessor->m_YR);
  ASSERT_EQ(0x00, m_pProcessor->m_SR & NEGATIVE);
  ASSERT_EQ(ZERO, m_pProcessor->m_SR & ZERO);

  m_pProcessor->Tick();

  ASSERT_EQ(0x85, m_pProcessor->m_YR);
  ASSERT_EQ(NEGATIVE, m_pProcessor->m_SR & NEGATIVE);
  ASSERT_EQ(0x00, m_pProcessor->m_SR & ZERO);

  ASSERT_EQ(0xF006, m_pProcessor->m_PC);
}


/* 0xA2 */
TEST_F(MOS6502Test, OpcodeLDX_Immediate) {
  uint8_t instr[] = {0xA2, 0x02,  /* LDX #02 */
                     0xA2, 0x00,  /* LDX #00 */
                     0xA2, 0x85}; /* LDX #85 */
  m_pMemory->Load(0xf000, instr, sizeof instr);

  m_pProcessor->Tick();

  ASSERT_EQ(0x02, m_pProcessor->m_XR);
  ASSERT_EQ(0x00, m_pProcessor->m_SR & NEGATIVE);
  ASSERT_EQ(0x00, m_pProcessor->m_SR & ZERO);

  m_pProcessor->Tick();

  ASSERT_EQ(0x00, m_pProcessor->m_XR);
  ASSERT_EQ(0x00, m_pProcessor->m_SR & NEGATIVE);
  ASSERT_EQ(ZERO, m_pProcessor->m_SR & ZERO);

  m_pProcessor->Tick();

  ASSERT_EQ(0x85, m_pProcessor->m_XR);
  ASSERT_EQ(NEGATIVE, m_pProcessor->m_SR & NEGATIVE);
  ASSERT_EQ(0x00, m_pProcessor->m_SR & ZERO);

  ASSERT_EQ(0xF006, m_pProcessor->m_PC);
}

/* 0xA4 */
TEST_F(MOS6502Test, OpcodeLDY_ZPG) {
  uint8_t instr[] = {0xA4, 0x85,  /* LDY $85 */
                     0xA4, 0x85,  /* LDY $85 */
                     0xA4, 0x85}; /* LDY $85 */
  m_pMemory->Load(0xf000, instr, sizeof instr);
  m_pMemory->Load(0x85, 0x55);
  m_pProcessor->Tick();

  ASSERT_EQ(0x55, m_pProcessor->m_YR);
  ASSERT_EQ(0x00, m_pProcessor->m_SR & NEGATIVE);
  ASSERT_EQ(0x00, m_pProcessor->m_SR & ZERO);

  m_pMemory->Load(0x85, 0x80);
  m_pProcessor->Tick();

  ASSERT_EQ(0x80, m_pProcessor->m_YR);
  ASSERT_EQ(NEGATIVE, m_pProcessor->m_SR & NEGATIVE);
  ASSERT_EQ(0x00, m_pProcessor->m_SR & ZERO);

  m_pMemory->Load(0x85, 0x00);
  m_pProcessor->Tick();

  ASSERT_EQ(0x00, m_pProcessor->m_YR);
  ASSERT_EQ(0x00, m_pProcessor->m_SR & NEGATIVE);
  ASSERT_EQ(ZERO, m_pProcessor->m_SR & ZERO);

  ASSERT_EQ(0xF006, m_pProcessor->m_PC);
}

/* 0xA5 */
TEST_F(MOS6502Test, OpcodeLDA_ZPG) {
  uint8_t instr[] = {0xA5, 0x85,  /* LDA $85 */
                     0xA5, 0x85,  /* LDA $85 */
                     0xA5, 0x85}; /* LDA $85 */
  m_pMemory->Load(0xf000, instr, sizeof instr);
  m_pMemory->Load(0x85, 0x55);
  m_pProcessor->Tick();

  ASSERT_EQ(0x55, m_pProcessor->m_AC);
  ASSERT_EQ(0x00, m_pProcessor->m_SR & NEGATIVE);
  ASSERT_EQ(0x00, m_pProcessor->m_SR & ZERO);

  m_pMemory->Load(0x85, 0x80);
  m_pProcessor->Tick();

  ASSERT_EQ(0x80, m_pProcessor->m_AC);
  ASSERT_EQ(NEGATIVE, m_pProcessor->m_SR & NEGATIVE);
  ASSERT_EQ(0x00, m_pProcessor->m_SR & ZERO);

  m_pMemory->Load(0x85, 0x00);
  m_pProcessor->Tick();

  ASSERT_EQ(0x00, m_pProcessor->m_AC);
  ASSERT_EQ(0x00, m_pProcessor->m_SR & NEGATIVE);
  ASSERT_EQ(ZERO, m_pProcessor->m_SR & ZERO);

  ASSERT_EQ(0xF006, m_pProcessor->m_PC);
}

/* 0xA6 */
TEST_F(MOS6502Test, OpcodeLDX_ZPG) {
  uint8_t instr[] = {0xA6, 0x85,  /* LDX $85 */
                     0xA6, 0x85,  /* LDX $85 */
                     0xA6, 0x85}; /* LDX $85 */
  m_pMemory->Load(0xf000, instr, sizeof instr);
  m_pMemory->Load(0x85, 0x55);
  m_pProcessor->Tick();

  ASSERT_EQ(0x55, m_pProcessor->m_XR);
  ASSERT_EQ(0x00, m_pProcessor->m_SR & NEGATIVE);
  ASSERT_EQ(0x00, m_pProcessor->m_SR & ZERO);

  m_pMemory->Load(0x85, 0x80);
  m_pProcessor->Tick();

  ASSERT_EQ(0x80, m_pProcessor->m_XR);
  ASSERT_EQ(NEGATIVE, m_pProcessor->m_SR & NEGATIVE);
  ASSERT_EQ(0x00, m_pProcessor->m_SR & ZERO);

  m_pMemory->Load(0x85, 0x00);
  m_pProcessor->Tick();

  ASSERT_EQ(0x00, m_pProcessor->m_XR);
  ASSERT_EQ(0x00, m_pProcessor->m_SR & NEGATIVE);
  ASSERT_EQ(ZERO, m_pProcessor->m_SR & ZERO);

  ASSERT_EQ(0xF006, m_pProcessor->m_PC);
}

/* 0xA8 */
TEST_F(MOS6502Test, OpcodeTAY) {
  uint8_t instr[] = {0xA8,   /* TAY */
                     0xA8,   /* TAY */
                     0xA8};  /* TAY */
  m_pMemory->Load(0xf000, instr, sizeof instr);

  m_pProcessor->m_AC = 0x55;
  m_pProcessor->Tick();

  ASSERT_EQ(0x55, m_pProcessor->m_YR);
  ASSERT_EQ(0x00, m_pProcessor->m_SR & NEGATIVE);
  ASSERT_EQ(0x00, m_pProcessor->m_SR & ZERO);

  m_pProcessor->m_AC = 0x00;
  m_pProcessor->Tick();

  ASSERT_EQ(0x00, m_pProcessor->m_YR);
  ASSERT_EQ(0x00, m_pProcessor->m_SR & NEGATIVE);
  ASSERT_EQ(ZERO, m_pProcessor->m_SR & ZERO);

  m_pProcessor->m_AC = 0x80;
  m_pProcessor->Tick();

  ASSERT_EQ(0x80, m_pProcessor->m_YR);
  ASSERT_EQ(NEGATIVE, m_pProcessor->m_SR & NEGATIVE);
  ASSERT_EQ(0x00, m_pProcessor->m_SR & ZERO);

  ASSERT_EQ(0xF003, m_pProcessor->m_PC);
}

/* 0xA9 */
TEST_F(MOS6502Test, OpcodeLDA) {
  uint8_t instr[] = {0xA9, 0x02,  /* LDA #02 */
                     0xA9, 0x00,  /* LDA #00 */
                     0xA9, 0x85}; /* LDA #80 */
  m_pMemory->Load(0xf000, instr, sizeof instr);

  m_pProcessor->Tick();

  ASSERT_EQ(0x02, m_pProcessor->m_AC);
  ASSERT_EQ(0x00, m_pProcessor->m_SR & NEGATIVE);
  ASSERT_EQ(0x00, m_pProcessor->m_SR & ZERO);

  m_pProcessor->Tick();

  ASSERT_EQ(0x00, m_pProcessor->m_AC);
  ASSERT_EQ(0x00, m_pProcessor->m_SR & NEGATIVE);
  ASSERT_EQ(ZERO, m_pProcessor->m_SR & ZERO);

  m_pProcessor->Tick();

  ASSERT_EQ(0x85, m_pProcessor->m_AC);
  ASSERT_EQ(NEGATIVE, m_pProcessor->m_SR & NEGATIVE);
  ASSERT_EQ(0x00, m_pProcessor->m_SR & ZERO);

  ASSERT_EQ(0xF006, m_pProcessor->m_PC);
}

/* 0xAA */
TEST_F(MOS6502Test, OpcodeTAX) {
  uint8_t instr[] = {0xAA,   /* TAX */
                     0xAA,   /* TAX */
                     0xAA};  /* TAX */
  m_pMemory->Load(0xf000, instr, sizeof instr);

  m_pProcessor->m_AC = 0x55;
  m_pProcessor->Tick();

  ASSERT_EQ(0x55, m_pProcessor->m_XR);
  ASSERT_EQ(0x00, m_pProcessor->m_SR & NEGATIVE);
  ASSERT_EQ(0x00, m_pProcessor->m_SR & ZERO);

  m_pProcessor->m_AC = 0x00;
  m_pProcessor->Tick();

  ASSERT_EQ(0x00, m_pProcessor->m_XR);
  ASSERT_EQ(0x00, m_pProcessor->m_SR & NEGATIVE);
  ASSERT_EQ(ZERO, m_pProcessor->m_SR & ZERO);

  m_pProcessor->m_AC = 0x80;
  m_pProcessor->Tick();

  ASSERT_EQ(0x80, m_pProcessor->m_XR);
  ASSERT_EQ(NEGATIVE, m_pProcessor->m_SR & NEGATIVE);
  ASSERT_EQ(0x00, m_pProcessor->m_SR & ZERO);

  ASSERT_EQ(0xF003, m_pProcessor->m_PC);
}

/* 0xAC */
TEST_F(MOS6502Test, OpcodeLDY_ABS) {
  uint8_t instr[] = {0xAC, 0x85, 0x00,  /* LDY $85 */
                     0xAC, 0x85, 0x00,  /* LDY $85 */
                     0xAC, 0x85, 0x00}; /* LDY $85 */
  m_pMemory->Load(0xf000, instr, sizeof instr);
  m_pMemory->Load(0x85, 0x55);
  m_pProcessor->Tick();

  ASSERT_EQ(0x55, m_pProcessor->m_YR);
  ASSERT_EQ(0x00, m_pProcessor->m_SR & NEGATIVE);
  ASSERT_EQ(0x00, m_pProcessor->m_SR & ZERO);

  m_pMemory->Load(0x85, 0x80);
  m_pProcessor->Tick();

  ASSERT_EQ(0x80, m_pProcessor->m_YR);
  ASSERT_EQ(NEGATIVE, m_pProcessor->m_SR & NEGATIVE);
  ASSERT_EQ(0x00, m_pProcessor->m_SR & ZERO);

  m_pMemory->Load(0x85, 0x00);
  m_pProcessor->Tick();

  ASSERT_EQ(0x00, m_pProcessor->m_YR);
  ASSERT_EQ(0x00, m_pProcessor->m_SR & NEGATIVE);
  ASSERT_EQ(ZERO, m_pProcessor->m_SR & ZERO);

  ASSERT_EQ(0xF009, m_pProcessor->m_PC);
}

/* 0xAD */
TEST_F(MOS6502Test, OpcodeLDA_ABS) {
  uint8_t instr[] = {0xAD, 0x85, 0x00,  /* LDA $85 */
                     0xAD, 0x85, 0x00,  /* LDA $85 */
                     0xAD, 0x85, 0x00}; /* LDA $85 */
  m_pMemory->Load(0xf000, instr, sizeof instr);
  m_pMemory->Load(0x85, 0x55);
  m_pProcessor->Tick();

  ASSERT_EQ(0x55, m_pProcessor->m_AC);
  ASSERT_EQ(0x00, m_pProcessor->m_SR & NEGATIVE);
  ASSERT_EQ(0x00, m_pProcessor->m_SR & ZERO);

  m_pMemory->Load(0x85, 0x80);
  m_pProcessor->Tick();

  ASSERT_EQ(0x80, m_pProcessor->m_AC);
  ASSERT_EQ(NEGATIVE, m_pProcessor->m_SR & NEGATIVE);
  ASSERT_EQ(0x00, m_pProcessor->m_SR & ZERO);

  m_pMemory->Load(0x85, 0x00);
  m_pProcessor->Tick();

  ASSERT_EQ(0x00, m_pProcessor->m_AC);
  ASSERT_EQ(0x00, m_pProcessor->m_SR & NEGATIVE);
  ASSERT_EQ(ZERO, m_pProcessor->m_SR & ZERO);

  ASSERT_EQ(0xF009, m_pProcessor->m_PC);
}

/* 0xAE */
TEST_F(MOS6502Test, OpcodeLDX_ABS) {
  uint8_t instr[] = {0xAE, 0x85, 0x00,  /* LDX $85 */
                     0xAE, 0x85, 0x00,  /* LDX $85 */
                     0xAE, 0x85, 0x00}; /* LDX $85 */
  m_pMemory->Load(0xf000, instr, sizeof instr);
  m_pMemory->Load(0x85, 0x55);
  m_pProcessor->Tick();

  ASSERT_EQ(0x55, m_pProcessor->m_XR);
  ASSERT_EQ(0x00, m_pProcessor->m_SR & NEGATIVE);
  ASSERT_EQ(0x00, m_pProcessor->m_SR & ZERO);

  m_pMemory->Load(0x85, 0x80);
  m_pProcessor->Tick();

  ASSERT_EQ(0x80, m_pProcessor->m_XR);
  ASSERT_EQ(NEGATIVE, m_pProcessor->m_SR & NEGATIVE);
  ASSERT_EQ(0x00, m_pProcessor->m_SR & ZERO);

  m_pMemory->Load(0x85, 0x00);
  m_pProcessor->Tick();

  ASSERT_EQ(0x00, m_pProcessor->m_XR);
  ASSERT_EQ(0x00, m_pProcessor->m_SR & NEGATIVE);
  ASSERT_EQ(ZERO, m_pProcessor->m_SR & ZERO);

  ASSERT_EQ(0xF009, m_pProcessor->m_PC);
}

/* 0xB0 */
TEST_F(MOS6502Test, OpcodeBCS) {
  uint8_t instr[] = {0xB0, 0xFB,  /* BCS -5*/
                     0xB0, 0xFB,  /* BCS -5 */
                     0xB0, 0x05,  /* BCS 5 */
                     0xB0, 0x05}; /* BCS 5 */

  m_pMemory->Load(0xf000, instr, sizeof instr);

  m_pProcessor->m_SR &= ~CARRY;

  m_pProcessor->Tick();
  ASSERT_EQ(0xF002, m_pProcessor->m_PC);

  m_pProcessor->m_SR |= CARRY;
  m_pProcessor->Tick();
  ASSERT_EQ(0xEFFF, m_pProcessor->m_PC);

  m_pProcessor->m_PC = 0xF004;
  m_pProcessor->Tick();
  ASSERT_EQ(0xF00B, m_pProcessor->m_PC);

  m_pProcessor->m_SR &= ~CARRY;
  m_pProcessor->m_PC = 0xF006;
  m_pProcessor->Tick();
  ASSERT_EQ(0xF008, m_pProcessor->m_PC);
}

/* 0xB8 */
TEST_F(MOS6502Test, OpcodeCLV) {
  m_pMemory->Load(0xf000, 0xB8); /* CLV */

  m_pProcessor->m_SR |= OVERFLOW;
  m_pProcessor->Tick();

  ASSERT_EQ(0x00, m_pProcessor->m_SR & OVERFLOW);
  ASSERT_EQ(0xF001, m_pProcessor->m_PC);
}

/* 0xBC */
TEST_F(MOS6502Test, OpcodeLDY_ABS_X) {
  uint8_t instr[] = {0xBC, 0x80, 0x00,   /* LDY $80, X */
                     0xBC, 0x80, 0x00,   /* LDY $80, X */
                     0xBC, 0x80, 0x00,   /* LDY $80, X */
                     0xBC, 0x80, 0x00 }; /* LDY $80, X */

  m_pMemory->Load(0xf000, instr, sizeof instr);
  m_pMemory->Load(0x81, 0x20);
  m_pProcessor->m_XR = 0x01;

  m_pProcessor->Tick();

  ASSERT_EQ(0x20, m_pProcessor->m_YR);
  ASSERT_EQ(0x00, m_pProcessor->m_SR & NEGATIVE);
  ASSERT_EQ(0x00, m_pProcessor->m_SR & ZERO);

  m_pMemory->Load(0x81, 0x80);
  m_pProcessor->Tick();

  ASSERT_EQ(0x80, m_pProcessor->m_YR);
  ASSERT_EQ(NEGATIVE, m_pProcessor->m_SR & NEGATIVE);
  ASSERT_EQ(0x00, m_pProcessor->m_SR & ZERO);

  m_pMemory->Load(0x81, 0x00);
  m_pProcessor->Tick();

  ASSERT_EQ(0x00, m_pProcessor->m_YR);
  ASSERT_EQ(0x00, m_pProcessor->m_SR & NEGATIVE);
  ASSERT_EQ(ZERO, m_pProcessor->m_SR & ZERO);

  m_pProcessor->Tick();

  ASSERT_EQ(0x00, m_pProcessor->m_YR);
  ASSERT_EQ(0x00, m_pProcessor->m_SR & NEGATIVE);
  ASSERT_EQ(ZERO, m_pProcessor->m_SR & ZERO);

  ASSERT_EQ(0xF00C, m_pProcessor->m_PC);
}

/* 0xBD */
TEST_F(MOS6502Test, OpcodeLDA_ABS_X) {
  uint8_t instr[] = {0xBD, 0x80, 0x00,   /* LDA $80, X */
                     0xBD, 0x80, 0x00,   /* LDA $80, X */
                     0xBD, 0x80, 0x00,   /* LDA $80, X */
                     0xBD, 0x80, 0x00 }; /* LDA $80, X */

  m_pMemory->Load(0xf000, instr, sizeof instr);
  m_pMemory->Load(0x81, 0x20);
  m_pProcessor->m_XR = 0x01;

  m_pProcessor->Tick();

  ASSERT_EQ(0x20, m_pProcessor->m_AC);
  ASSERT_EQ(0x00, m_pProcessor->m_SR & NEGATIVE);
  ASSERT_EQ(0x00, m_pProcessor->m_SR & ZERO);

  m_pMemory->Load(0x81, 0x80);
  m_pProcessor->Tick();

  ASSERT_EQ(0x80, m_pProcessor->m_AC);
  ASSERT_EQ(NEGATIVE, m_pProcessor->m_SR & NEGATIVE);
  ASSERT_EQ(0x00, m_pProcessor->m_SR & ZERO);

  m_pMemory->Load(0x81, 0x00);
  m_pProcessor->Tick();

  ASSERT_EQ(0x00, m_pProcessor->m_AC);
  ASSERT_EQ(0x00, m_pProcessor->m_SR & NEGATIVE);
  ASSERT_EQ(ZERO, m_pProcessor->m_SR & ZERO);

  m_pProcessor->Tick();

  ASSERT_EQ(0x00, m_pProcessor->m_AC);
  ASSERT_EQ(0x00, m_pProcessor->m_SR & NEGATIVE);
  ASSERT_EQ(ZERO, m_pProcessor->m_SR & ZERO);

  ASSERT_EQ(0xF00C, m_pProcessor->m_PC);
}

/* 0xBE */
TEST_F(MOS6502Test, OpcodeLDX_ABS_Y) {
  uint8_t instr[] = {0xBE, 0x80, 0x00,   /* LDX $80, Y */
                     0xBE, 0x80, 0x00,   /* LDX $80, Y */
                     0xBE, 0x80, 0x00,   /* LDX $80, Y */
                     0xBE, 0x80, 0x00 }; /* LDX $80, Y */

  m_pMemory->Load(0xf000, instr, sizeof instr);
  m_pMemory->Load(0x81, 0x20);
  m_pProcessor->m_YR = 0x01;

  m_pProcessor->Tick();

  ASSERT_EQ(0x20, m_pProcessor->m_XR);
  ASSERT_EQ(0x00, m_pProcessor->m_SR & NEGATIVE);
  ASSERT_EQ(0x00, m_pProcessor->m_SR & ZERO);

  m_pMemory->Load(0x81, 0x80);
  m_pProcessor->Tick();

  ASSERT_EQ(0x80, m_pProcessor->m_XR);
  ASSERT_EQ(NEGATIVE, m_pProcessor->m_SR & NEGATIVE);
  ASSERT_EQ(0x00, m_pProcessor->m_SR & ZERO);

  m_pMemory->Load(0x81, 0x00);
  m_pProcessor->Tick();

  ASSERT_EQ(0x00, m_pProcessor->m_XR);
  ASSERT_EQ(0x00, m_pProcessor->m_SR & NEGATIVE);
  ASSERT_EQ(ZERO, m_pProcessor->m_SR & ZERO);

  m_pProcessor->Tick();

  ASSERT_EQ(0x00, m_pProcessor->m_XR);
  ASSERT_EQ(0x00, m_pProcessor->m_SR & NEGATIVE);
  ASSERT_EQ(ZERO, m_pProcessor->m_SR & ZERO);

  ASSERT_EQ(0xF00C, m_pProcessor->m_PC);
}

/* 0xCA */
TEST_F(MOS6502Test, OpcodeDEX) {
  uint8_t instr[] = {0xCA,  /* DEX */
                     0xCA,  /* DEX */
                     0xCA}; /* DEX */
  m_pMemory->Load(0xf000, instr, sizeof instr);
  m_pProcessor->m_XR = 0x02;

  m_pProcessor->Tick();

  ASSERT_EQ(0x01, m_pProcessor->m_XR);
  ASSERT_EQ(0x00, m_pProcessor->m_SR & NEGATIVE);
  ASSERT_EQ(0x00, m_pProcessor->m_SR & ZERO);

  m_pProcessor->Tick();

  ASSERT_EQ(0x00, m_pProcessor->m_XR);
  ASSERT_EQ(0x00, m_pProcessor->m_SR & NEGATIVE);
  ASSERT_EQ(ZERO, m_pProcessor->m_SR & ZERO);

  m_pProcessor->Tick();

  ASSERT_EQ(0xFF, m_pProcessor->m_XR);
  ASSERT_EQ(NEGATIVE, m_pProcessor->m_SR & NEGATIVE);
  ASSERT_EQ(0x00, m_pProcessor->m_SR & ZERO);

  ASSERT_EQ(0xF003, m_pProcessor->m_PC);
}

/* 0xCD */
TEST_F(MOS6502Test, OpcodeCMP_ABS) {
  uint8_t instr[] = {0xCD, 0x00, 0xF0,   /* CMP $f000 */
                     0xCD, 0x00, 0xF0,   /* CMP $f000 */
                     0xCD, 0x00, 0xF0};  /* CMP $f000 */


  m_pMemory->Load(0xf000, instr, sizeof instr);

  m_pProcessor->m_AC = 0xCE;

  m_pProcessor->Tick();

  ASSERT_EQ(0x00, m_pProcessor->m_SR & NEGATIVE);
  ASSERT_EQ(0x00, m_pProcessor->m_SR & CARRY);
  ASSERT_EQ(0x00, m_pProcessor->m_SR & ZERO);

  m_pProcessor->m_AC = 0xCD;
  m_pProcessor->Tick();

  ASSERT_EQ(0x00, m_pProcessor->m_SR & NEGATIVE);
  ASSERT_EQ(0x00, m_pProcessor->m_SR & CARRY);
  ASSERT_EQ(ZERO, m_pProcessor->m_SR & ZERO);

  m_pProcessor->m_AC = 0xCC;
  m_pProcessor->Tick();

  ASSERT_EQ(NEGATIVE, m_pProcessor->m_SR & NEGATIVE);
  ASSERT_EQ(CARRY, m_pProcessor->m_SR & CARRY);
  ASSERT_EQ(0x00, m_pProcessor->m_SR & ZERO);

  ASSERT_EQ(0xf009, m_pProcessor->m_PC);
}

/* 0xD0 */
TEST_F(MOS6502Test, OPcodeBNE) {
  uint8_t instr[] = {0xD0, 0xFB,  /* BNE -5*/
                     0xD0, 0xFB,  /* BNE -5 */
                     0xD0, 0x05,  /* BNE 5 */
                     0xD0, 0x05}; /* BNE 5 */

  m_pMemory->Load(0xf000, instr, sizeof instr);

  m_pProcessor->m_SR |= ZERO;

  m_pProcessor->Tick();
  ASSERT_EQ(0xF002, m_pProcessor->m_PC);

  m_pProcessor->m_SR &= ~ZERO;
  m_pProcessor->Tick();
  ASSERT_EQ(0xEFFF, m_pProcessor->m_PC);

  m_pProcessor->m_PC = 0xF004;
  m_pProcessor->Tick();
  ASSERT_EQ(0xF00B, m_pProcessor->m_PC);

  m_pProcessor->m_SR |= ZERO;
  m_pProcessor->m_PC = 0xF006;
  m_pProcessor->Tick();
  ASSERT_EQ(0xF008, m_pProcessor->m_PC);
}


/* 0xD5 */
TEST_F(MOS6502Test, OpcodeCMP_ZPG_X) {
  uint8_t instr[] = {0xD5, 0x80,   /* CMP $80, X */
                     0xD5, 0x80,   /* CMP $80, X */
                     0xD5, 0x80};  /* CMP $80, X */


  m_pMemory->Load(0xf000, instr, sizeof instr);
  m_pMemory->Load(0x81, 0x01);
  m_pProcessor->m_AC = 0x02;
  m_pProcessor->m_XR = 0x01;

  m_pProcessor->Tick();

  ASSERT_EQ(0x00, m_pProcessor->m_SR & NEGATIVE);
  ASSERT_EQ(0x00, m_pProcessor->m_SR & CARRY);
  ASSERT_EQ(0x00, m_pProcessor->m_SR & ZERO);

  m_pProcessor->m_AC = 0x01;
  m_pProcessor->Tick();

  ASSERT_EQ(0x00, m_pProcessor->m_SR & NEGATIVE);
  ASSERT_EQ(0x00, m_pProcessor->m_SR & CARRY);
  ASSERT_EQ(ZERO, m_pProcessor->m_SR & ZERO);

  m_pProcessor->m_AC = 0x00;
  m_pProcessor->Tick();

  ASSERT_EQ(NEGATIVE, m_pProcessor->m_SR & NEGATIVE);
  ASSERT_EQ(CARRY, m_pProcessor->m_SR & CARRY);
  ASSERT_EQ(0x00, m_pProcessor->m_SR & ZERO);

  ASSERT_EQ(0xf006, m_pProcessor->m_PC);
}

/* 0xD8 */
TEST_F(MOS6502Test, OpcodeCLD) {
  m_pMemory->Load(0xf000, 0xD8); /* CLD */

  m_pProcessor->m_SR |= DECIMAL;
  m_pProcessor->Tick();

  ASSERT_EQ(0x00, m_pProcessor->m_SR & DECIMAL);
  ASSERT_EQ(0xF001, m_pProcessor->m_PC);
}

/* 0xD9 */
TEST_F(MOS6502Test, OpcodeCMP_ABS_Y) {
  uint8_t instr[] = {0xD9, 0x80, 0x00,   /* CMP $80, Y */
                     0xD9, 0x80, 0x00,   /* CMP $80, Y */
                     0xD9, 0x80, 0x00};  /* CMP $80, Y */


  m_pMemory->Load(0xf000, instr, sizeof instr);
  m_pMemory->Load(0x81, 0x01);
  m_pProcessor->m_AC = 0x02;
  m_pProcessor->m_YR = 0x01;

  m_pProcessor->Tick();

  ASSERT_EQ(0x00, m_pProcessor->m_SR & NEGATIVE);
  ASSERT_EQ(0x00, m_pProcessor->m_SR & CARRY);
  ASSERT_EQ(0x00, m_pProcessor->m_SR & ZERO);

  m_pProcessor->m_AC = 0x01;
  m_pProcessor->Tick();

  ASSERT_EQ(0x00, m_pProcessor->m_SR & NEGATIVE);
  ASSERT_EQ(0x00, m_pProcessor->m_SR & CARRY);
  ASSERT_EQ(ZERO, m_pProcessor->m_SR & ZERO);

  m_pProcessor->m_AC = 0x00;
  m_pProcessor->Tick();

  ASSERT_EQ(NEGATIVE, m_pProcessor->m_SR & NEGATIVE);
  ASSERT_EQ(CARRY, m_pProcessor->m_SR & CARRY);
  ASSERT_EQ(0x00, m_pProcessor->m_SR & ZERO);
}

/* 0xDD */
TEST_F(MOS6502Test, OpcodeCMP_ABS_X) {
  uint8_t instr[] = {0xDD, 0x80, 0x00,   /* CMP $80, X */
                     0xDD, 0x80, 0x00,   /* CMP $80, X */
                     0xDD, 0x80, 0x00};  /* CMP $80, X */


  m_pMemory->Load(0xf000, instr, sizeof instr);
  m_pMemory->Load(0x81, 0x01);
  m_pProcessor->m_AC = 0x02;
  m_pProcessor->m_XR = 0x01;

  m_pProcessor->Tick();

  ASSERT_EQ(0x00, m_pProcessor->m_SR & NEGATIVE);
  ASSERT_EQ(0x00, m_pProcessor->m_SR & CARRY);
  ASSERT_EQ(0x00, m_pProcessor->m_SR & ZERO);

  m_pProcessor->m_AC = 0x01;
  m_pProcessor->Tick();

  ASSERT_EQ(0x00, m_pProcessor->m_SR & NEGATIVE);
  ASSERT_EQ(0x00, m_pProcessor->m_SR & CARRY);
  ASSERT_EQ(ZERO, m_pProcessor->m_SR & ZERO);

  m_pProcessor->m_AC = 0x00;
  m_pProcessor->Tick();

  ASSERT_EQ(NEGATIVE, m_pProcessor->m_SR & NEGATIVE);
  ASSERT_EQ(CARRY, m_pProcessor->m_SR & CARRY);
  ASSERT_EQ(0x00, m_pProcessor->m_SR & ZERO);
}

/* 0xE8 */
TEST_F(MOS6502Test, OpcodeINX) {
  uint8_t instr[] = {0xE8,  /* INX */
                     0xE8,  /* INX */
                     0xE8}; /* INX */

  m_pMemory->Load(0xf000, instr, sizeof instr);
  m_pProcessor->m_XR = 0xFE;

  m_pProcessor->Tick();
  ASSERT_EQ(0xFF, m_pProcessor->m_XR);
  ASSERT_EQ(NEGATIVE, m_pProcessor->m_SR & NEGATIVE);
  ASSERT_EQ(0x00, m_pProcessor->m_SR & ZERO);

  m_pProcessor->Tick();
  ASSERT_EQ(0x00, m_pProcessor->m_XR);
  ASSERT_EQ(0x00, m_pProcessor->m_SR & NEGATIVE);
  ASSERT_EQ(ZERO, m_pProcessor->m_SR & ZERO);

  m_pProcessor->Tick();
  ASSERT_EQ(0x01, m_pProcessor->m_XR);
  ASSERT_EQ(0x00, m_pProcessor->m_SR & NEGATIVE);
  ASSERT_EQ(0x00, m_pProcessor->m_SR & ZERO);

  ASSERT_EQ(0xF003, m_pProcessor->m_PC);
}

/* 0xEA */
TEST_F(MOS6502Test, OpcodeNOP) {
  m_pMemory->Load(0xf000, 0xEA); /* NOP */

  m_pProcessor->Tick();

  ASSERT_EQ(0xF001, m_pProcessor->m_PC);
}

/* 0xF0 */
TEST_F(MOS6502Test, OPcodeBEQ) {
  uint8_t instr[] = {0xF0, 0xFB,  /* BEQ -5*/
                     0xF0, 0xFB,  /* BEQ -5 */
                     0xF0, 0x05,  /* BEQ 5 */
                     0xF0, 0x05}; /* BEQ 5 */

  m_pMemory->Load(0xf000, instr, sizeof instr);

  m_pProcessor->m_SR &= ~ZERO;

  m_pProcessor->Tick();
  ASSERT_EQ(0xF002, m_pProcessor->m_PC);

  m_pProcessor->m_SR |= ZERO;
  m_pProcessor->Tick();
  ASSERT_EQ(0xEFFF, m_pProcessor->m_PC);

  m_pProcessor->m_PC = 0xF004;
  m_pProcessor->Tick();
  ASSERT_EQ(0xF00B, m_pProcessor->m_PC);

  m_pProcessor->m_SR &= ~ZERO;
  m_pProcessor->m_PC = 0xF006;
  m_pProcessor->Tick();
  ASSERT_EQ(0xF008, m_pProcessor->m_PC);
}

/* 0xF8 */
TEST_F(MOS6502Test, OpcodeSED) {
  m_pMemory->Load(0xf000, 0xF8); /* SED */

  m_pProcessor->Tick();

  ASSERT_EQ(DECIMAL, m_pProcessor->m_SR & DECIMAL);

  ASSERT_EQ(0xF001, m_pProcessor->m_PC);
}

/* 0xF9 */
TEST_F(MOS6502Test, OpcodeSBC_ABS_Y) {
  uint8_t instr[] = {0xF9, 0x7F, 0x00,  /* SBC $7F, Y */
                     0xF9, 0x7F, 0x00,  /* SBC $7F, Y */
                     0xF9, 0x7F, 0x00}; /* SBC $7F, Y */

  m_pMemory->Load(0xf000, instr, sizeof instr);
  m_pMemory->Load(0x80, 0x20);

  m_pProcessor->m_AC = 0x80;
  m_pProcessor->m_SR &= ~CARRY;
  m_pProcessor->m_YR = 0x01;

  m_pProcessor->Tick();

  ASSERT_EQ(0x5F, m_pProcessor->m_AC);
  ASSERT_EQ(0x00, m_pProcessor->m_SR & ZERO);
  ASSERT_EQ(0x00, m_pProcessor->m_SR & NEGATIVE);
  ASSERT_EQ(CARRY, m_pProcessor->m_SR & CARRY);
  ASSERT_EQ(OVERFLOW, m_pProcessor->m_SR & OVERFLOW);

  m_pMemory->Load(0x80, 0x5F);
  m_pProcessor->Tick();

  ASSERT_EQ(0x00, m_pProcessor->m_AC);
  ASSERT_EQ(ZERO, m_pProcessor->m_SR & ZERO);
  ASSERT_EQ(0x00, m_pProcessor->m_SR & NEGATIVE);
  ASSERT_EQ(CARRY, m_pProcessor->m_SR & CARRY);
  ASSERT_EQ(0x00, m_pProcessor->m_SR & OVERFLOW);

  m_pProcessor->m_AC = 0x20;
  m_pProcessor->Tick();

  ASSERT_EQ(0xC1, m_pProcessor->m_AC);
  ASSERT_EQ(0x00, m_pProcessor->m_SR & ZERO);
  ASSERT_EQ(NEGATIVE, m_pProcessor->m_SR & NEGATIVE);
  ASSERT_EQ(0x00, m_pProcessor->m_SR & CARRY);
  ASSERT_EQ(0x00, m_pProcessor->m_SR & OVERFLOW);
}

/* 0xFD */
TEST_F(MOS6502Test, OpcodeSBC_ABS_X) {
  uint8_t instr[] = {0xFD, 0x7F, 0x00,  /* SBC $7F, X */
                     0xFD, 0x7F, 0x00,  /* SBC $7F, X */
                     0xFD, 0x7F, 0x00}; /* SBC $7F, X */

  m_pMemory->Load(0xf000, instr, sizeof instr);
  m_pMemory->Load(0x80, 0x20);

  m_pProcessor->m_AC = 0x80;
  m_pProcessor->m_SR &= ~CARRY;
  m_pProcessor->m_XR = 0x01;

  m_pProcessor->Tick();

  ASSERT_EQ(0x5F, m_pProcessor->m_AC);
  ASSERT_EQ(0x00, m_pProcessor->m_SR & ZERO);
  ASSERT_EQ(0x00, m_pProcessor->m_SR & NEGATIVE);
  ASSERT_EQ(CARRY, m_pProcessor->m_SR & CARRY);
  ASSERT_EQ(OVERFLOW, m_pProcessor->m_SR & OVERFLOW);

  m_pMemory->Load(0x80, 0x5F);
  m_pProcessor->Tick();

  ASSERT_EQ(0x00, m_pProcessor->m_AC);
  ASSERT_EQ(ZERO, m_pProcessor->m_SR & ZERO);
  ASSERT_EQ(0x00, m_pProcessor->m_SR & NEGATIVE);
  ASSERT_EQ(CARRY, m_pProcessor->m_SR & CARRY);
  ASSERT_EQ(0x00, m_pProcessor->m_SR & OVERFLOW);

  m_pProcessor->m_AC = 0x20;
  m_pProcessor->Tick();

  ASSERT_EQ(0xC1, m_pProcessor->m_AC);
  ASSERT_EQ(0x00, m_pProcessor->m_SR & ZERO);
  ASSERT_EQ(NEGATIVE, m_pProcessor->m_SR & NEGATIVE);
  ASSERT_EQ(0x00, m_pProcessor->m_SR & CARRY);
  ASSERT_EQ(0x00, m_pProcessor->m_SR & OVERFLOW);
}
