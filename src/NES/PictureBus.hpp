#pragma once
#include <vector>
#include "Mapper.hpp"
    
class PictureBus
{
	public:

		PictureBus();

		uint8_t Read(uint16_t addr) const;
		uint8_t ReadPalette(uint8_t paletteAddr) const;
		void 	Write(uint16_t addr, uint8_t value);

		bool 	SetMapper(std::shared_ptr<Mapper> mapper, uint8_t mirroringMode);

		void 	UpdateMirroring(NameTableMirroring mirroringMode);

	private:
		size_t NameTable0, NameTable1, NameTable2, NameTable3; //indices where they start in RAM vector

		std::vector<uint8_t> m_RAM;
		std::vector<uint8_t> m_palette;

		std::shared_ptr<Mapper> m_mapper;
};
