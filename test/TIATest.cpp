/****************************************************************************
* Unit Test Includes
*****************************************************************************/
#include "gtest/gtest.h"

#include "Memory.h"
#include "TIACore.h"

/*****************************************************************************
* Unit Tests
*****************************************************************************/

class MOS6502Test : public ::testing::Test {
protected:
  void SetUp() override {
    m_pMemory = new Memory();
    m_pMemory->Init();
    m_pMemory->Reset();

    m_pMemory->SetTIA(m_pTIA);
  }

  void TearDown() override {
    delete(m_pMemory);
  }

  TIACore *m_pTIA;
  Memory *m_pMemory;
};