#pragma once

#include <cstdint>
#include <memory>
#include <vector>

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

	static std::unique_ptr<Mapper> Create(uint8_t mapperNum, std::vector<uint8_t>&& PRG_ROM, std::vector<uint8_t>&& CHR_ROM);

protected:
	std::vector<uint8_t> m_CHR_ROM;
	std::vector<uint8_t> m_PRG_ROM;
};
