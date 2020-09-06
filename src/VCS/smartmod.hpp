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

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
template<unsigned base>
uint8_t smartmod(uint8_t x)
{
  return x % base;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
template<>
inline uint8_t smartmod<2>(uint8_t x)
{
  return x & 0x01;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
template<>
inline uint8_t smartmod<4>(uint8_t x)
{
  return x & 0x03;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
template<>
inline uint8_t smartmod<8>(uint8_t x)
{
  return x & 0x07;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
template<>
inline uint8_t smartmod<16>(uint8_t x)
{
  return x & 0x0F;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
template<>
inline uint8_t smartmod<32>(uint8_t x)
{
  return x & 0x1F;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
template<>
inline uint8_t smartmod<64>(uint8_t x)
{
  return x & 0x3F;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
template<>
inline uint8_t smartmod<128>(uint8_t x)
{
  return x & 0x7F;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
template<>
inline uint8_t smartmod<256>(uint8_t x)
{
  return x & 0xFF;
}
