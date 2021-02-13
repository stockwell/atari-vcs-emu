#pragma once

#include "NESMemory.hpp"

#include <cstdint>
#include <memory>
#include <vector>

class NESMemory;

class Mapper
{
public:
	Mapper() = default;
	virtual ~Mapper() = default;

	virtual void 	WritePRG(uint16_t address, uint8_t value) = 0;
	virtual uint8_t ReadPRG(uint16_t address) = 0;

	virtual void 	WriteCHR(uint16_t address, uint8_t value) = 0;
	virtual uint8_t ReadCHR(uint16_t address) = 0;

	virtual const uint8_t* GetPagePtr(uint16_t addr) = 0;

	static std::shared_ptr<Mapper> Create(std::shared_ptr<NESMemory> pMemory);
};
