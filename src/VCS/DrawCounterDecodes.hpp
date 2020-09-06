#pragma once

#include <cstdint>

class DrawCounterDecodes
{
public:
	DrawCounterDecodes(const DrawCounterDecodes &) = delete;
	DrawCounterDecodes(DrawCounterDecodes &&) = delete;
	DrawCounterDecodes &operator=(const DrawCounterDecodes &) = delete;
	DrawCounterDecodes &operator=(DrawCounterDecodes &&) = delete;

	const uint8_t *const *playerDecodes() const;
	const uint8_t *const *missileDecodes() const;

	static DrawCounterDecodes &get();

protected:
	DrawCounterDecodes();

private:
	uint8_t *myPlayerDecodes[8]{nullptr};

	uint8_t *myMissileDecodes[8]{nullptr};

	uint8_t myDecodes0[160];
	uint8_t myDecodes1[160];
	uint8_t myDecodes2[160];
	uint8_t myDecodes3[160];
	uint8_t myDecodes4[160];
	uint8_t myDecodes6[160];

	static DrawCounterDecodes myInstance;
};
