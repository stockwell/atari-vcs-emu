#pragma once

#include <memory>

#include "Common.h"

#include "TIABase.hpp"
#include "TIAPlayer.hpp"

class TIAPlayfield : public TIABase
{
public:
	TIAPlayfield(std::shared_ptr<TIAPlayer> Player0, std::shared_ptr<TIAPlayer> Player1)
		: m_Player0(std::move(Player0))
		, m_Player1(std::move(Player1))
	{
	}

	void UpdatePixel(uint16_t currentPos, uint8_t *pixel)
	{
		bool pixelEnabled = false;
		bool secondHalf = false;

		uint32_t PF = (m_PF0 >> 4u) | (m_PF1 << 4u) | (m_PF2 << 12u);

		if (currentPos < 148) {
			if (PF & (1u << ((currentPos - 68u) >> 2u))) {
				pixelEnabled = true;
			}
		} else {
			// Check if PF is reversed
			if ((m_CTRL & 0x01u) == 0x01u) {
				if (PF & (1u << (19 - ((currentPos - 148u) >> 2)))) {
					pixelEnabled = true;
				}
			} else {
				if (PF & (1u << (((currentPos - 148u)) >> 2u))) {
					pixelEnabled = true;
					secondHalf = true;
				}
			}
		}

		if (!pixelEnabled)
			return;

		if ((m_CTRL & 0x02u) == 0x02u) {
			if (secondHalf) {
				*pixel = m_Player1->GetColour();
			} else {
				*pixel = m_Player0->GetColour();
			}
		} else {
			*pixel = m_Colour;
		}
	};

	void SetPF0(uint8_t val)  { m_PF0 = val; };
	void SetPF1(uint8_t val)  { m_PF1 = bitreverse(val); };
	void SetPF2(uint8_t val)  { m_PF2 = val; };
	void SetCTRL(uint8_t val) { m_CTRL = val; };

private:
	std::shared_ptr<TIAPlayer> m_Player0;
	std::shared_ptr<TIAPlayer> m_Player1;

	uint8_t m_PF0  = 0x00;
	uint8_t m_PF1  = 0x00;
	uint8_t m_PF2  = 0x00;
	uint8_t m_CTRL = 0x00;
};