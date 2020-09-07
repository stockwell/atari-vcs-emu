#pragma once

#include "Common.hpp"

#include "TIABase.hpp"

class TIACore;

class TIAPlayer : public TIABase
{
public:
	explicit TIAPlayer(TIACore* pTIA, uint32_t collisionMask);

	uint8_t GetPos();
	void ResetPos(uint8_t value);

	void SetSize(uint8_t value, bool hblank);
	void SetGraphics(uint8_t value);
	void SetReflected(bool value);
	void SetVdelay(uint8_t value);
	void SetDivider(uint8_t divider);
	void ShufflePatterns();

	// TIABase
	void Tick(uint32_t x, uint32_t hcount) override;
	void MovementTick(uint32_t clock, uint8_t hclock, bool hblank) override;
	void NextLine() override;

private:
	void UpdatePattern();

private:
	static constexpr int8_t kRenderCounterOffset = -5;

	bool m_Reflected = false;
	bool m_isSuppressed = false;
	bool m_isDelaying = false;

	uint8_t m_DecodesOffset = 0;

	uint8_t m_Pattern = 0;
	uint8_t m_PatternOld = 0;
	uint8_t m_PatternNew = 0;

	uint8_t m_SampleCounter = 0;
	uint8_t m_Divider = 0;
	uint8_t m_DividerPending = 0;
	int8_t m_DividerChangeCounter = -1;
	int8_t m_RenderCounterTripPoint = 0;

	const uint8_t* m_Decodes = nullptr;

	TIACore* m_TIA = nullptr;
};
