#include <stdexcept>

#include "Mapper.hpp"
#include "Mappers/NROM.hpp"

std::unique_ptr<Mapper> Mapper::Create(uint8_t mapperNum, std::vector<uint8_t>&& PRG_ROM, std::vector<uint8_t>&& CHR_ROM)
{
	switch(mapperNum)
	{
	case 0:
		return std::make_unique<NROM>(std::move(PRG_ROM), std::move(CHR_ROM));
	}

	throw std::runtime_error("Unsupported mapper type!");
}