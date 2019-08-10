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
    }

    void TearDown() override {
      delete(m_pMemory);
      delete(m_pProcessor);
    }

    Memory *m_pMemory;
    MOS6502Core *m_pProcessor;

};

TEST_F(MOS6502Test, ResetTest) {
  m_pProcessor->m_pMemory->Load(0xfffd, 0xAA);
  m_pProcessor->m_pMemory->Load(0xfffc, 0x55);

  m_pProcessor->Reset();

  ASSERT_EQ(m_pProcessor->m_SP, 0xFD);
  ASSERT_EQ(m_pProcessor->m_PC, 0xAA55);

}

TEST_F(MOS6502Test, OpcodeSEI) {
  m_pProcessor->m_pMemory->Load(0xfffd, 0xF0);
  m_pProcessor->m_pMemory->Load(0xfffc, 0x00);
  m_pProcessor->m_pMemory->Load(0xf000, 0x78);

  m_pProcessor->Reset();

  m_pProcessor->Tick();

  ASSERT_EQ(m_pProcessor->m_SR & INTERRUPT, INTERRUPT);
}
