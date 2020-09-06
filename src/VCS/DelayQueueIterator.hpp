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

class DelayQueueIterator
{
public:
	virtual ~DelayQueueIterator() = default;

public:
	virtual bool isValid() const = 0;

	virtual uint8_t delay() const = 0;

	virtual uint8_t address() const = 0;

	virtual uint8_t value() const = 0;

	virtual bool next() = 0;
};

#endif // TIA_DELAY_QUEUE_ITERATOR
