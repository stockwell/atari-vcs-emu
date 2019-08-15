#ifndef ATARI_VCS_EMU_TIACORE_H
#define ATARI_VCS_EMU_TIACORE_H

#include "Common.h"

class TIACore {
public:
    TIACore();
    ~TIACore();
    void Write(uint8_t address, uint8_t value);
    uint8_t Read(uint8_t address);
};

static const char* kTIAReadRegisterNames[0x0F] = {
    "Cxm0p",    /* Read Collision M0-P1   M0-P0 */
    "Cxm0p",    /*                M1-P0   M1-P1 */
    "Cxp0fb",   /*                P0-PF   P0-BL */
    "Cxp1fb",   /*                P1-PF   P1-BL */
    "Cxm0fb",   /*                M0-PF   M0-BL */
    "Cxm1fb",   /*                M1-PF   M1-BL */
    "Cxblpf",   /*                BL-PF   ----- */
    "Cxppmm",   /*                P0-P1   M0-M1 */
    "Intp0",    /* Read Pot Port 0 */
    "Intp1",    /* Read Pot Port 1 */
    "Intp2",    /* Read Pot Port 2 */
    "Intp3",    /* Read Pot Port 3 */
    "Intp4",    /* Read Input (Trigger) 0 */
    "Intp5"     /* Read Input (Trigger) 1 */
};


/*
00      Vsync           0000 00x0       Vertical Sync Set-Clear
01      Vblank          xx00 00x0       Vertical Blank Set-Clear
02      Wsync           ---- ----       Wait for Horizontal Blank
03      Rsync           ---- ----       Reset Horizontal Sync Counter
04      Nusiz0          00xx 0xxx       Number-Size player/missle 0
05      Nusiz1          00xx 0xxx       Number-Size player/missle 1
06      Colup0          xxxx xxx0       Color-Luminance Player 0
07      Colup1          xxxx xxx0       Color-Luminance Player 1
08      Colupf          xxxx xxx0       Color-Luminance Playfield
09      Colubk          xxxx xxx0       Color-Luminance Background
0A      Ctrlpf          00xx 0xxx       Control Playfield, Ball, Collisions
0B      Refp0           0000 x000       Reflection Player 0
0C      Refp1           0000 x000       Reflection Player 1
0D      Pf0             xxxx 0000       Playfield Register Byte 0
0E      Pf1             xxxx xxxx       Playfield Register Byte 1
0F      Pf2             xxxx xxxx       Playfield Register Byte 2
10      Resp0           ---- ----       Reset Player 0
11      Resp1           ---- ----       Reset Player 1
12      Resm0           ---- ----       Reset Missle 0
13      Resm1           ---- ----       Reset Missle 1
14      Resbl           ---- ----       Reset Ball
15      Audc0           0000 xxxx       Audio Control 0
16      Audc1           0000 xxxx       Audio Control 1
17      Audf0           000x xxxx       Audio Frequency 0
18      Audf1           000x xxxx       Audio Frequency 1
19      Audv0           0000 xxxx       Audio Volume 0
1A      Audv1           0000 xxxx       Audio Volume 1
1B      Grp0            xxxx xxxx       Graphics Register Player 0
1C      Grp1            xxxx xxxx       Graphics Register Player 1
1D      Enam0           0000 00x0       Graphics Enable Missle 0
1E      Enam1           0000 00x0       Graphics Enable Missle 1
1F      Enabl           0000 00x0       Graphics Enable Ball
20      Hmp0            xxxx 0000       Horizontal Motion Player 0
21      Hmp1            xxxx 0000       Horizontal Motion Player 1
22      Hmm0            xxxx 0000       Horizontal Motion Missle 0
23      Hmm1            xxxx 0000       Horizontal Motion Missle 1
24      Hmbl            xxxx 0000       Horizontal Motion Ball
25      Vdelp0          0000 000x       Vertical Delay Player 0
26      Vdelp1          0000 000x       Vertical Delay Player 1
27      Vdelbl          0000 000x       Vertical Delay Ball
28      Resmp0          0000 00x0       Reset Missle 0 to Player 0
29      Resmp1          0000 00x0       Reset Missle 1 to Player 1
2A      Hmove           ---- ----       Apply Horizontal Motion
2B      Hmclr           ---- ----       Clear Horizontal Move Registers
2C      Cxclr           ---- ----       Clear Collision Latches */

#endif //ATARI_VCS_EMU_TIACORE_H
