#pragma once

static const char *kOPCodeNames[0x100] =
{
	/* 0x00 */
	"BRK impl",
	"ORA X,ind",
	"unused opcode",
	"unused opcode",
	"unused opcode",
	"ORA zpg",
	"ASL zpg",
	"unused opcode",
	"PHP impl",
	"ORA #",
	"ASL A",
	"unused opcode",
	"unused opcode",
	"ORA abs",
	"ASL abs",
	"unused opcode",

	/* 0x10 */
	"BPL rel",
	"ORA ind, Y",
	"unused opcode",
	"unused opcode",
	"unused opcode",
	"ORA zpg, X",
	"ASL zpg, X",
	"unused opcode",
	"CLC, impl",
	"ORA abs, Y",
	"unused opcode",
	"unused opcode",
	"unused opcode",
	"ORA abs, X",
	"ASL abs, X",
	"unused opcode",

	/* 0x20 */
	"JSR abs",
	"AND X, ind",
	"unused opcode",
	"unused opcode",
	"BIT zpg",
	"AND zpg",
	"ROL zpg",
	"unused opcode",
	"PLP impl",
	"AND #",
	"ROL A",
	"unused opcode",
	"BIT abs",
	"AND abs",
	"ROL abs",
	"unused opcode",

	/* 0x30 */
	"BMI rel",
	"AND ind, Y",
	"unused opcode",
	"unused opcode",
	"unused opcode",
	"AND zpg, X",
	"ROL zpg, X",
	"unused opcode",
	"SEC impl",
	"AND abs, Y",
	"unused opcode",
	"unused opcode",
	"unused opcode",
	"AND abs, X",
	"ROL abs, X",
	"unused opcode",

	/* 0x40 */
	"RTI impl",
	"EOR x ind",
	"unused opcode",
	"unused opcode",
	"unused opcode",
	"EOR zpg",
	"LSR zpg",
	"unused opcode",
	"PHA, impl",
	"EOR #",
	"LSR A",
	"unused opcode",
	"JMP abs",
	"EOR abs",
	"LSR abs",
	"unused opcode",

	/* 0x50 */
	"BVC rel",
	"EOR ind, Y",
	"unused opcode",
	"unused opcode",
	"unused opcode",
	"EOR zpg, x",
	"LSR zpg, X",
	"unused opcode",
	"CLI impl",
	"EOR abs, Y",
	"unused opcode",
	"unused opcode",
	"unused opcode",
	"EOR abs, X",
	"LSR abs, X",
	"unused opcode",

	/* 0x60 */
	"RTS impl",
	"ADC X, ind",
	"unused opcode",
	"unused opcode",
	"unused opcode",
	"ADC zpg",
	"ROR zpg",
	"unused opcode",
	"PLA impl",
	"ADC #",
	"ROR A",
	"unused opcode",
	"JMP ind",
	"ADC abs",
	"ROR abs",
	"unused opcode",

	/* 0x70 */
	"BVS rel",
	"ADC ind, Y",
	"unused opcode",
	"unused opcode",
	"unused opcode",
	"ADC zpg, X",
	"ROR zpg, X",
	"unused opcode",
	"SEI impl",
	"ADC abs, Y",
	"unused opcode",
	"unused opcode",
	"unused opcode",
	"ADC abs, X",
	"ROR abs, X",
	"unused opcode",

	/* 0x80 */
	"unused opcode",
	"STA X, ind",
	"unused opcode",
	"unused opcode",
	"STY zpg",
	"STA zpg",
	"STX zpg",
	"unused opcode",
	"DEY impl",
	"unused opcode",
	"TXA impl",
	"unused opcode",
	"STY abs",
	"STA abs",
	"STX abs",
	"unused opcode",

	/* 0x90 */
	"BCC rel",
	"STA ind, Y",
	"unused opcode",
	"unused opcode",
	"STY zpg, X",
	"STA zpg, X",
	"STX zpg, Y",
	"unused opcode",
	"TYA impl",
	"STA abs, Y",
	"TXS impl",
	"unused opcode",
	"unused opcode",
	"STA abs, X",
	"unused opcode",
	"unused opcode",

	/* 0xA0 */
	"LDY #",
	"LDA X, ind",
	"LDX #",
	"unused opcode",
	"LDY zpg",
	"LDA zpg",
	"LDX zpg",
	"unused opcode",
	"TAY impl",
	"LDA #",
	"TAX impl",
	"unused opcode",
	"LDY abs",
	"LDA abs",
	"LDX abs",
	"unused opcode",

	/* 0xB0 */
	"BCS rel",
	"LDA ind, Y",
	"unused opcode",
	"unused opcode",
	"LDY zpg, X",
	"LDA zpg, X",
	"LDX zpg, Y",
	"unused opcode",
	"CLV impl",
	"LDA abs, Y",
	"TSX impl",
	"unused opcode",
	"LDY abs, X",
	"LDA abs, X",
	"LDX abs, Y",
	"unused opcode",

	/* 0xC0 */
	"CPY #",
	"CMP X, ind",
	"unused opcode",
	"unused opcode",
	"CPY zpg",
	"CMP zpg",
	"DEC zpg",
	"unused opcode",
	"INY impl",
	"CMP #",
	"DEX impl",
	"unused opcode",
	"CPY abs",
	"CMP abs",
	"DEC abs",
	"unused opcode",

	/* 0xD0 */
	"BNE rel",
	"CMP ind, Y",
	"unused opcode",
	"unused opcode",
	"unused opcode",
	"CMP zpg, X",
	"DEC zpg, X",
	"unused opcode",
	"CLD impl",
	"CMP abs, Y",
	"unused opcode",
	"unused opcode",
	"unused opcode",
	"CMP abs, X",
	"DEC abs, X",
	"unused opcode",

	/* 0xE0 */
	"CPX #",
	"SBC X, ind",
	"unused opcode",
	"unused opcode",
	"CPX zpg",
	"SBC zpg",
	"INC zpg",
	"unused opcode",
	"INX impl",
	"SBC #",
	"NOP impl",
	"unused opcode",
	"CPX abs",
	"SBC abs",
	"INC abs",
	"unused opcode",

	/* 0xF0 */
	"BEQ rel",
	"SBC ind, Y",
	"unused opcode",
	"unused opcode",
	"unused opcode",
	"SBC zpg, X",
	"INC zpg, X",
	"unused opcode",
	"SED impl",
	"SBC abs, Y",
	"unused opcode",
	"unused opcode",
	"unused opcode",
	"SBC abs, X",
	"INC abs, X",
	"unused opcode",
};

static const uint8_t cycletime[0x100] =
{
	21, 18, 0, 0, 0, 9, 15, 0, 9, 6, 6, 0, 0, 12, 18, 0,   // 00
	6,  15, 0, 0, 0, 12, 18, 0, 6, 12, 0, 0, 0, 12, 21, 0, // 10
	18, 18, 0, 0, 9, 9, 15, 0, 12, 6, 6, 0, 12, 12, 18, 0, // 20
	6,  15, 0, 0, 0, 12, 18, 0, 6, 12, 0, 0, 0, 12, 21, 0, // 30
	18, 18, 0, 0, 0, 9, 15, 0, 9, 6, 6, 0, 9, 12, 18, 0,   // 40
	6,  15, 0, 0, 0, 12, 18, 0, 6, 12, 0, 0, 0, 12, 21, 0, // 50
	18, 18, 0, 0, 0, 9, 15, 0, 12, 6, 6, 0, 15, 12, 18, 0, // 60
	6, 15, 0, 0, 0, 12, 18, 0, 6, 12, 0, 0, 0, 12, 21, 0, // 70
	0, 18, 0, 0, 9, 9, 9, 0, 6, 0, 6, 0, 12, 12, 12, 0, // 80
	6, 18, 0, 0, 12, 12, 12, 0, 6, 15, 6, 0, 0, 15, 0, 0, // 90
	6, 18, 6, 0, 9, 9, 9, 0, 6, 6, 6, 0, 12, 12, 12, 0, // A0
	6, 15, 0, 0, 12, 12, 12, 0, 6, 12, 6, 0, 12, 12, 12, 0, // B0
	6, 18, 0, 0, 9, 9, 15, 0, 6, 6, 6, 0, 12, 12, 9, 0, // C0
	6, 15, 0, 0, 0, 12, 18, 0, 6, 12, 0, 0, 0, 12, 21, 0, // D0
	6, 18, 0, 0, 9, 9, 15, 0, 6, 6, 6, 0, 12, 12, 18, 0, // E0
	6, 15, 0, 0, 0, 12, 18, 0, 6, 12, 0, 0, 0, 12, 21, 0, // F0
};
