#include <stdexcept>

#include "Mapper.hpp"
#include "Mappers/NROM.hpp"
#include "Mappers/AOROM.hpp"
#include "Mappers/SxROM.hpp"

std::shared_ptr<Mapper> Mapper::Create(const std::shared_ptr<NESMemory>& pMemory, std::function<void(NameTableMirroring&)> mirroring_cb)
{
	switch(pMemory->GetMapperType())
	{
		case 0:
			return std::make_shared<NROM>(pMemory->GetPRG(), pMemory->GetCHR());

		case 1:
			return std::make_shared<SxROM>(pMemory->GetPRG(), pMemory->GetCHR(), mirroring_cb);

		case 7:
			return std::make_shared<AOROM>(pMemory->GetPRG(), pMemory->GetCHR(), mirroring_cb);
	}

	throw std::runtime_error("Unsupported mapper type!");
}
