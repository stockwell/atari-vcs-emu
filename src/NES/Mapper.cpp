#include <stdexcept>

#include "Mapper.hpp"
#include "Mappers/NROM.hpp"

std::shared_ptr<Mapper> Mapper::Create(std::shared_ptr<NESMemory> pMemory)
{
	switch(pMemory->GetMapperType())
	{
		case 0:
			return std::make_shared<NROM>(pMemory->GetPRG(), pMemory->GetCHR());
	}

	throw std::runtime_error("Unsupported mapper type!");
}