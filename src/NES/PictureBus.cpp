#include "PictureBus.hpp"

PictureBus::PictureBus()
	: m_RAM(0x800)
	, m_palette(0x20)
{

}

uint8_t PictureBus::Read(uint16_t addr) const
{
	if (addr < 0x2000)
	{
		return m_mapper->ReadCHR(addr);
	}
	else if (addr < 0x3eff) //Name tables upto 0x3000, then mirrored upto 3eff
	{
		auto index = addr & 0x3ff;
		if (addr < 0x2400)      //NT0
			return m_RAM[NameTable0 + index];
		else if (addr < 0x2800) //NT1
			return m_RAM[NameTable1 + index];
		else if (addr < 0x2c00) //NT2
			return m_RAM[NameTable2 + index];
		else                    //NT3
			return m_RAM[NameTable3 + index];
	}
	else if (addr < 0x3fff)
	{
		return m_palette[addr & 0x1f];
	}
	return 0;
}

uint8_t PictureBus::ReadPalette(uint8_t paletteAddr) const
{
	return m_palette[paletteAddr];
}

void PictureBus::Write(uint16_t addr, uint8_t value)
{
	if (addr < 0x2000)
	{
		m_mapper->WriteCHR(addr, value);
	}
	else if (addr < 0x3eff) //Name tables upto 0x3000, then mirrored upto 3eff
	{
		auto index = addr & 0x3ff;
		if (addr < 0x2400)      //NT0
			m_RAM[NameTable0 + index] = value;
		else if (addr < 0x2800) //NT1
			m_RAM[NameTable1 + index] = value;
		else if (addr < 0x2c00) //NT2
			m_RAM[NameTable2 + index] = value;
		else                    //NT3
			m_RAM[NameTable3 + index] = value;
	}
	else if (addr < 0x3fff)
	{
		if (addr == 0x3f10)
			m_palette[0] = value;
		else
			m_palette[addr & 0x1f] = value;
	}
}

void PictureBus::UpdateMirroring(NameTableMirroring mirroringMode)
{
	switch (mirroringMode)
	{
		case NameTableMirroring::Horizontal:
			NameTable0 = NameTable1 = 0;
			NameTable2 = NameTable3 = 0x400;
			break;

		case NameTableMirroring::Vertical:
			NameTable0 = NameTable2 = 0;
			NameTable1 = NameTable3 = 0x400;
			break;

		case NameTableMirroring::OneScreenLower:
			NameTable0 = NameTable1 = NameTable2 = NameTable3 = 0;
			break;

		case NameTableMirroring::OneScreenHigher:
			NameTable0 = NameTable1 = NameTable2 = NameTable3 = 0x400;
			break;

		default:
			NameTable0 = NameTable1 = NameTable2 = NameTable3 = 0;
			printf("Unsupported Name Table mirroring : %d\n", mirroringMode);
	}
}

bool PictureBus::SetMapper(std::shared_ptr<Mapper> mapper, uint8_t mirroringMode)
{
	if (! mapper)
	{
		printf("Mapper argument is nullptr\n");
		return false;
	}

	m_mapper = std::move(mapper);
	UpdateMirroring(static_cast<NameTableMirroring>(mirroringMode));
	return true;
}

