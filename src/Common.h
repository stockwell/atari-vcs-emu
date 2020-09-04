#pragma once

#include <cstdint>
#include <cstdio>

#include "HexDump.h"

#ifdef DEBUG
	#define Log(M, ...) printf(M "\n", ##__VA_ARGS__)
	#define LogHexDump(...) hexDump(__VA_ARGS__)
#else
	#define Log(msg, ...) (void)0
	#define LogHexDump(...) (void)0
#endif

static uint8_t lookup[16] =
{
	0x0, 0x8, 0x4, 0xc, 0x2, 0xa, 0x6, 0xe,
	0x1, 0x9, 0x5, 0xd, 0x3, 0xb, 0x7, 0xf,
};

inline uint8_t bitreverse(uint8_t n)
{
	// Reverse the top and bottom nibble then swap them.
	return (lookup[n & 0b1111] << 4) | lookup[n >> 4];
}
