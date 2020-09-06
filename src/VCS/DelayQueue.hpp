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

#include "smartmod.hpp"
#include "DelayQueueMember.hpp"

template<unsigned length, unsigned capacity>
class DelayQueueIteratorImpl;

template<unsigned length, unsigned capacity>
class DelayQueue
{
public:
	friend DelayQueueIteratorImpl<length, capacity>;

public:
	DelayQueue();

public:

	void push(uint8_t address, uint8_t value, uint8_t delay);

	void reset();

	template<typename T> void execute(T executor);

private:
	std::array<DelayQueueMember<capacity>, length> myMembers;
	uint8_t myIndex{0};
	std::array<uint8_t, 0xFF> myIndices;

private:
	DelayQueue(const DelayQueue &) = delete;
	DelayQueue(DelayQueue &&) = delete;
	DelayQueue &operator=(const DelayQueue &) = delete;
	DelayQueue &operator=(DelayQueue &&) = delete;
};

// ############################################################################
// Implementation
// ############################################################################

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
template<unsigned length, unsigned capacity>
DelayQueue<length, capacity>::DelayQueue()
{
	myIndices.fill(0xFF);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
template<unsigned length, unsigned capacity>
void DelayQueue<length, capacity>::push(uint8_t address, uint8_t value, uint8_t delay)
{
	if (delay >= length)
		throw std::runtime_error("delay exceeds queue length");

	uint8_t currentIndex = myIndices[address];

	if (currentIndex < length)
		myMembers[currentIndex].remove(address);

	uint8_t index = smartmod<length>(myIndex + delay);
	myMembers[index].push(address, value);

	myIndices[address] = index;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
template<unsigned length, unsigned capacity>
void DelayQueue<length, capacity>::reset()
{
	for (uint32_t i = 0; i < length; ++i)
		myMembers[i].clear();

	myIndex = 0;
	myIndices.fill(0xFF);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
template<unsigned length, unsigned capacity>
template<typename T>
void DelayQueue<length, capacity>::execute(T executor)
{
	DelayQueueMember<capacity> &currentMember = myMembers[myIndex];

	for (uint8_t i = 0; i < currentMember.mySize; ++i) {
		executor(currentMember.myEntries[i].address, currentMember.myEntries[i].value);
		myIndices[currentMember.myEntries[i].address] = 0xFF;
	}

	currentMember.clear();

	myIndex = smartmod<length>(myIndex + 1);
}
