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

template<unsigned capacity>
class DelayQueueMember
{

public:
	struct Entry
	{
		uint8_t address{0};
		uint8_t value{0};
	};

public:
	DelayQueueMember();

public:
	void push(uint8_t address, uint8_t value);

	void remove(uint8_t address);

	void clear();

public:
	std::array<Entry, capacity> myEntries;
	uint8_t mySize{0};

private:
	DelayQueueMember(const DelayQueueMember<capacity> &) = delete;
	DelayQueueMember(DelayQueueMember<capacity> &&) = delete;
	DelayQueueMember<capacity> &operator=(const DelayQueueMember<capacity> &) = delete;
	DelayQueueMember<capacity> &operator=(DelayQueueMember<capacity> &&) = delete;

};

// ############################################################################
// Implementation
// ############################################################################

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
template<unsigned capacity>
DelayQueueMember<capacity>::DelayQueueMember()
{
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
template<unsigned capacity>
void DelayQueueMember<capacity>::push(uint8_t address, uint8_t value)
{
	if (mySize == capacity)
		throw std::runtime_error("delay queue overflow");

	myEntries[mySize].address = address;
	myEntries[mySize++].value = value;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
template<unsigned capacity>
void DelayQueueMember<capacity>::remove(uint8_t address)
{
	uint8_t index;

	for (index = 0; index < mySize; ++index) {
		if (myEntries[index].address == address) break;
	}

	if (index < mySize) {
		for (uint8_t i = index + 1; i < mySize; ++i) {
			myEntries[i - 1] = myEntries[i];
		}

		--mySize;
	}
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
template<unsigned capacity>
void DelayQueueMember<capacity>::clear()
{
	mySize = 0;
}
