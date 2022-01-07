#pragma once

#include <functional>
#include <array>
#include <cstdint>

#include "PictureBus.hpp"
#include "Memory.hpp"

const int ScanlineCycleLength = 341;
const int ScanlineEndCycle = 340;
const int VisibleScanlines = 240;
const int ScanlineVisibleDots = 256;
const int FrameEndScanline = 261;

const int AttributeOffset = 0x3C0;

class PPU
{
public:
	explicit PPU(std::shared_ptr<PictureBus> pBus);

	bool Tick(std::vector<uint8_t>& framebuffer);
	void reset();

	void SetInterruptCallback(std::function<void(bool)> cb);
	void SetA12Callback(std::function<void(uint8_t v)> cb);

	void doDMA(const uint8_t* page_ptr);

	//Callbacks mapped to CPU address space
	//Addresses written to by the program
	void control(uint8_t ctrl);
	void setMask(uint8_t mask);
	void setOAMAddress(uint8_t address);
	void setDataAddress(uint8_t address);
	void setScroll(uint8_t scroll);
	void setData(uint8_t data);

	//Read by the program
	uint8_t getStatus();
	uint8_t getData();
	uint8_t getOAMData() const;
	void setOAMData(uint8_t value);
	
private:
	uint8_t readOAM(uint8_t address) const;
	void writeOAM(uint8_t address, uint8_t value);
	uint8_t read(uint16_t address) const;

	std::shared_ptr<PictureBus> m_pBus;

	std::function<void(bool)> m_vblankCallback;
	std::function<void(uint8_t v)> m_A12Callback;

	std::vector<uint8_t> m_spriteMemory;
	std::vector<uint8_t> m_scanlineSprites;

	enum State
	{
		PreRender,
		Render,
		PostRender,
		VerticalBlank
	} m_pipelineState;

	int m_cycle;
	int m_scanline;
	bool m_evenFrame;

	bool m_vblank;
	bool m_sprZeroHit;

	//Registers
	uint16_t m_dataAddress;
	uint16_t m_tempAddress;
	uint8_t m_fineXScroll;
	bool m_firstWrite;
	uint8_t m_dataBuffer;

	uint8_t m_spriteDataAddress;

	//Setup flags and variables
	bool m_longSprites;
	bool m_generateInterrupt;

	bool m_greyscaleMode;
	bool m_showSprites;
	bool m_showBackground;
	bool m_hideEdgeSprites;
	bool m_hideEdgeBackground;

	enum CharacterPage
	{
		Low,
		High,
	} m_bgPage, m_sprPage;

	uint16_t m_dataAddrIncrement;
};
