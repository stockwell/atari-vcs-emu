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

#include "DelayQueue.hpp"
#include "DelayQueueIterator.hpp"

template<unsigned length, unsigned capacity>
class DelayQueueIteratorImpl: public DelayQueueIterator
{
public:
	explicit DelayQueueIteratorImpl(const DelayQueue<length, capacity> &delayQueue);

public:

	bool isValid() const override;

	uint8_t delay() const override;

	uint8_t address() const override;

	uint8_t value() const override;

	bool next() override;

private:
	uint8_t currentIndex() const;

private:
	const DelayQueue<length, capacity> &myDelayQueue;
	uint8_t myDelayCycle{0};
	uint8_t myIndex{0};
};

// ############################################################################
// Implementation
// ############################################################################

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
template<unsigned length, unsigned capacity>
DelayQueueIteratorImpl<length, capacity>::DelayQueueIteratorImpl(
	const DelayQueue<length, capacity> &delayQueue
)
	: myDelayQueue(delayQueue)
{
	while (myDelayQueue.myMembers[currentIndex()].mySize == 0 && isValid())
		myDelayCycle++;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
template<unsigned length, unsigned capacity>
bool DelayQueueIteratorImpl<length, capacity>::isValid() const
{
	return myDelayCycle < length;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
template<unsigned length, unsigned capacity>
uInt8 DelayQueueIteratorImpl<length, capacity>::delay() const
{
	if (!isValid()) {
		throw runtime_error("delay called on invalid DelayQueueInterator");
	}

	return myDelayCycle;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
template<unsigned length, unsigned capacity>
uInt8 DelayQueueIteratorImpl<length, capacity>::address() const
{
	if (!isValid()) {
		throw runtime_error("address called on invalid DelayQueueInterator");
	}

	return myDelayQueue.myMembers[currentIndex()].myEntries[myIndex].address;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
template<unsigned length, unsigned capacity>
uInt8 DelayQueueIteratorImpl<length, capacity>::value() const
{
	if (!isValid()) {
		throw runtime_error("value called on invalid DelayQueueInterator");
	}

	return myDelayQueue.myMembers[currentIndex()].myEntries[myIndex].value;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
template<unsigned length, unsigned capacity>
bool DelayQueueIteratorImpl<length, capacity>::next()
{
	if (!isValid()) return false;

	if (++myIndex < myDelayQueue.myMembers[currentIndex()].mySize)
		return true;

	myIndex = 0;

	do {
		++myDelayCycle;
	}
	while (myDelayQueue.myMembers[currentIndex()].mySize == 0 && isValid());

	return isValid();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
template<unsigned length, unsigned capacity>
uInt8 DelayQueueIteratorImpl<length, capacity>::currentIndex() const
{
	return (myDelayQueue.myIndex + myDelayCycle) % length;
}
