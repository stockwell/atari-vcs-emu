#ifndef ATARI_VCS_EMU_TIACORE_H
#define ATARI_VCS_EMU_TIACORE_H

#include "Common.h"

class TIACore {
public:
    TIACore();
    ~TIACore();
    void Init();
    void Write(uint8_t address, uint8_t value);
    uint8_t Read(uint8_t address);
};

static const char* kTIAReadRegisterNames[0x0E] = {
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

static const char* kTIAWriteRegisterNames[0x2D] = {
    "Vsync",  /* Vertical Sync Set-Clear              */
    "Vblank", /* Vertical Blank Set-Clear             */
    "Wsync",  /* Wait for Horizontal Blank            */
    "Rsync",  /* Reset Horizontal Sync Counter        */
    "Nusiz0", /* Number-Size player/missle 0          */
    "Nusiz1", /* Number-Size player/missle 1          */
    "Colup0", /* Color-Luminance Player 0             */
    "Colup1", /* Color-Luminance Player 1             */
    "Colupf", /* Color-Luminance Playfield            */
    "Colubk", /* Color-Luminance Background           */
    "Ctrlpf", /* Control Playfield, Ball, Collisions  */
    "Refp0",  /* Reflection Player 0                  */
    "Refp1",  /* Reflection Player 1                  */
    "Pf0",    /* Playfield Register Byte 0            */
    "Pf1",    /* Playfield Register Byte 1            */
    "Pf2",    /* Playfield Register Byte 2            */
    "Resp0",  /* Reset Player 0                       */
    "Resp1",  /* Reset Player 1                       */
    "Resm0",  /* Reset Missle 0                       */
    "Resm1",  /* Reset Missle 1                       */
    "Resbl",  /* Reset Ball                           */
    "Audc0",  /* Audio Control 0                      */
    "Audc1",  /* Audio Control 1                      */
    "Audf0",  /* Audio Frequency 0                    */
    "Audf1",  /* Audio Frequency 1                    */
    "Audv0",  /* Audio Volume 0                       */
    "Audv1",  /* Audio Volume 1                       */
    "Grp0",   /* Graphics Register Player 0           */
    "Grp1",   /* Graphics Register Player 1           */
    "Enam0",  /* Graphics Enable Missle 0             */
    "Enam1",  /* Graphics Enable Missle 1             */
    "Enabl",  /* Graphics Enable Ball                 */
    "Hmp0",   /* Horizontal Motion Player 0           */
    "Hmp1",   /* Horizontal Motion Player 1           */
    "Hmm0",   /* Horizontal Motion Missle 0           */
    "Hmm1",   /* Horizontal Motion Missle 1           */
    "Hmbl",   /* Horizontal Motion Ball               */
    "Vdelp0", /* Vertical Delay Player 0              */
    "Vdelp1", /* Vertical Delay Player 1              */
    "Vdelbl", /* Vertical Delay Ball                  */
    "Resmp0", /* Reset Missle 0 to Player 0           */
    "Resmp1", /* Reset Missle 1 to Player 1           */
    "Hmove",  /* Apply Horizontal Motion              */
    "Hmclr",  /* Clear Horizontal Move Registers      */
    "Cxclr"  /* Clear Collision Latches               */
};



#endif //ATARI_VCS_EMU_TIACORE_H
