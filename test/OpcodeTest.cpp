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
      m_pMemory->Init();
      m_pMemory->Reset();

      m_pProcessor = new MOS6502Core();
      m_pProcessor->Init(m_pMemory);
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
TEST_F(MOS6502Test, OpcodeORX) {

}

/* 0x05 */
TEST_F(MOS6502Test, OpcodeORA_ZPG) {

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

}

/* 0x15 */
TEST_F(MOS6502Test, OPcodeORA_ZPG_X) {

}

/* 0x16 */
TEST_F(MOS6502Test, OPcodeASL_ZPG_X) {

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

}

/* 0x20 */
TEST_F(MOS6502Test, OPcodeJSR) {

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

/* 0x40 */
TEST_F(MOS6502Test, OPCodePHA) {
  m_pMemory->Load(0xF000, 0x40); /* PHA */
  m_pProcessor->m_AC = 0x80;

  m_pProcessor->Tick();

  ASSERT_EQ(0xFC, m_pProcessor->m_SP);
  ASSERT_EQ(0x80, m_pMemory->Read(STACK_BASE + 0xFD));
  ASSERT_EQ(0xF001, m_pProcessor->m_PC);
}

/* 0x78 */
TEST_F(MOS6502Test, OpcodeSEI) {
  m_pMemory->Load(0xf000, 0x78); /* SEI */

  m_pProcessor->Tick();

  ASSERT_EQ(m_pProcessor->m_SR & INTERRUPT, INTERRUPT);

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

/* 0xA2 */
TEST_F(MOS6502Test, OpcodeLDX) {
  uint8_t instr[] = {0xA2, 0x02,  /* LDX #02 */
                     0xA2, 0x00,  /* LDX #00 */
                     0xA2, 0x85}; /* LDX #80 */
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

/* 0xF8 */
TEST_F(MOS6502Test, OpcodeSED) {
  m_pMemory->Load(0xf000, 0xF8); /* SED */

  m_pProcessor->Tick();

  ASSERT_EQ(DECIMAL, m_pProcessor->m_SR & DECIMAL);

  ASSERT_EQ(0xF001, m_pProcessor->m_PC);
}
