//============================================================================
//
//   SSSS    tt          lll  lll
//  SS  SS   tt           ll   ll
//  SS     tttttt  eeee   ll   ll   aaaa
//   SSSS    tt   ee  ee  ll   ll      aa
//      SS   tt   eeeeee  ll   ll   aaaaa  --  "An Atari 2600 VCS Emulator"
//  SS  SS   tt   ee      ll   ll  aa  aa
//   SSSS     ttt  eeeee llll llll  aaaaa
//
// Copyright (c) 1995-2020 by Bradford W. Mott, Stephen Anthony
// and the Stella Team
//
// See the file "License.txt" for information on usage and redistribution of
// this file, and for a DISCLAIMER OF ALL WARRANTIES.
//============================================================================

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
