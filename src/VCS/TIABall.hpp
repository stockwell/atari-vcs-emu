#pragma once

#include "TIABase.hpp"

class TIACore;

class TIABall : public TIABase
{
public:
	explicit TIABall(TIACore* pTIA);

	void ShuffleStatus();
	void SetVdelay(uint8_t value);
	void SetSize(uint8_t value);
	void ResetPos(uint8_t value);
	void UpdateEnabled();

	// TIABase
	void Tick(uint32_t x, uint32_t hcount) override;
	void MovementTick(uint8_t clock, uint8_t hclock, bool hblank) override;
	void NextLine() override;
	void SetEnable(bool enabled) override;

private:
	static constexpr int kRenderCounterOffset = -4;

	bool m_isEnabledOld = false;
	bool m_isEnabledNew = false;
	bool m_isDelaying = false;
	bool m_SignalActive = false;
	bool m_isReceivingRegularClock = false;

	uint8_t m_EffectiveWidth = 1;
	uint8_t m_Width = 1;
	uint8_t m_LastMovementTick = 0;

	TIACore* m_TIA = nullptr;
};