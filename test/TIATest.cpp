/****************************************************************************
* Unit Test Includes
*****************************************************************************/
#include "gtest/gtest.h"

#include "VCS/VCSMemory.hpp"
#include "VCS/TIACore.h"

/*****************************************************************************
* Unit Tests
*****************************************************************************/

class MOS6502Test
	: public ::testing::Test
{
protected:
	void SetUp() override
	{
		m_pMemory = std::make_shared<VCSMemory>();
		m_pMemory->Reset();

		m_pMemory->SetTIA(m_pTIA);
	}

	void TearDown() override
	{

	}

	std::shared_ptr<TIACore> m_pTIA = nullptr;
	std::shared_ptr<VCSMemory> m_pMemory = nullptr;
};