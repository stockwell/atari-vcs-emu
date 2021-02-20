#pragma once

#include <utility>
#include <vector>
#include <unordered_map>
#include <functional>

#include "Memory.hpp"
#include "Mapper.hpp"

class Mapper;

class NESMemory : public Memory
{
public:
	enum IORegisters
	{
		PPUCTRL = 0x2000,
		PPUMASK,
		PPUSTATUS,
		OAMADDR,
		OAMDATA,
		PPUSCROL,
		PPUADDR,
		PPUDATA,
		OAMDMA = 0x4014,
		JOY1 = 0x4016,
		JOY2 = 0x4017,
	};

	enum APURegisters
	{
		PULSE1 = 0x4000,
		PULSE2 = 0x4004,
		TRIANGLE = 0x4008,
		NOISE = 0x400C,
		APUSTATUS = 0x4015,
	};

	uint8_t	Read(uint16_t address) override;
	void 	Write(uint16_t address, uint8_t value) override;

	bool 	LoadROM(const uint8_t *pROM, uint16_t offset) override;

	void	SetMapper(std::shared_ptr<Mapper> pMapper);
	uint8_t	GetMapperType() const;

	uint8_t	GetNameTableMirroring() const;

	std::vector<uint8_t>* GetPRG();
	std::vector<uint8_t>* GetCHR();

	const uint8_t* GetPagePtr(uint8_t page);

	bool	SetWriteCallback(IORegisters reg, std::function<void(uint8_t)> callback);
	bool	SetReadCallback(IORegisters reg, std::function<uint8_t(void)> callback);

	void	SetAPUWriteCallback(std::function<void(uint16_t, uint8_t)> callback) { m_APUWriteCallback = std::move(callback); };
	void	SetAPUReadCallback(std::function<uint8_t(void)> callback) { m_APUReadCallback = std::move(callback); };

private:
	uint8_t m_mapperNumber 			= 0;
	uint8_t m_nameTableMirroring 	= 0;

	std::vector<uint8_t> 	m_extendedRAM;
	std::vector<uint8_t> 	m_CHR_ROM;
	std::vector<uint8_t> 	m_PRG_ROM;
	std::shared_ptr<Mapper>	m_pMapper = nullptr;

	std::unordered_map<IORegisters, std::function<void(uint8_t)>> m_writeCallbacks;
	std::unordered_map<IORegisters, std::function<uint8_t(void)>> m_readCallbacks;

	std::function<void(uint16_t, uint8_t)>	m_APUWriteCallback;
	std::function<uint8_t(void)> 			m_APUReadCallback;
};