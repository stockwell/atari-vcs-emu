#pragma once

#include "TIABase.hpp"

class TIACore;

class TIAMissile : public TIABase
{
public:
	explicit TIAMissile(TIACore* pTIA, uint32_t collisionMask);

	void SetSize(uint8_t value);
	void ResetPos(uint8_t value, bool hblank);
	void ResetPosPlayer(uint8_t value, uint8_t pos);
	void Tick(uint8_t hcount, bool isReceivingMclock = true);

	// TIABase
	void Tick(uint32_t x, uint32_t hcount) override { TIAMissile::Tick(hcount); }
	void MovementTick(uint32_t clock, uint8_t hclock, bool hblank) override;
	void NextLine() override;
	void SetEnable(bool enabled) override;

private:
	void UpdateEnabled();

private:
	static constexpr int kRenderCounterOffset = -4;

	bool m_isVisible = false;
	bool m_Enam = false;

	uint8_t m_EffectiveWidth = 1;
	uint8_t m_Width = 1;

	uint8_t m_Resmp = 0;

	uint8_t m_DecodesOffset = 0;
	const uint8_t* m_Decodes = nullptr;

	TIACore* m_TIA = nullptr;
};
