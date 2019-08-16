#ifndef ATARI_VCS_EMU_TIACORE_H
#define ATARI_VCS_EMU_TIACORE_H

#include "Common.h"

class TIACore {
public:
    TIACore();
    ~TIACore();
    void Init();
    void Write(uint16_t address, uint8_t value);
    uint8_t Read(uint16_t address);

private:
  void(TIACore::*m_WriteRegisters[0x2E])(uint8_t value);
  uint8_t *m_pMem;

private:
  void TIAWrite0x00(uint8_t value);
  void TIAWrite0x01(uint8_t value);
  void TIAWrite0x02(uint8_t value);
  void TIAWrite0x03(uint8_t value);
  void TIAWrite0x04(uint8_t value);
  void TIAWrite0x05(uint8_t value);
  void TIAWrite0x06(uint8_t value);
  void TIAWrite0x07(uint8_t value);
  void TIAWrite0x08(uint8_t value);
  void TIAWrite0x09(uint8_t value);
  void TIAWrite0x0A(uint8_t value);
  void TIAWrite0x0B(uint8_t value);
  void TIAWrite0x0C(uint8_t value);
  void TIAWrite0x0D(uint8_t value);
  void TIAWrite0x0E(uint8_t value);
  void TIAWrite0x0F(uint8_t value);

  void TIAWrite0x10(uint8_t value);
  void TIAWrite0x11(uint8_t value);
  void TIAWrite0x12(uint8_t value);
  void TIAWrite0x13(uint8_t value);
  void TIAWrite0x14(uint8_t value);
  void TIAWrite0x15(uint8_t value);
  void TIAWrite0x16(uint8_t value);
  void TIAWrite0x17(uint8_t value);
  void TIAWrite0x18(uint8_t value);
  void TIAWrite0x19(uint8_t value);
  void TIAWrite0x1A(uint8_t value);
  void TIAWrite0x1B(uint8_t value);
  void TIAWrite0x1C(uint8_t value);
  void TIAWrite0x1D(uint8_t value);
  void TIAWrite0x1E(uint8_t value);
  void TIAWrite0x1F(uint8_t value);

  void TIAWrite0x20(uint8_t value);
  void TIAWrite0x21(uint8_t value);
  void TIAWrite0x22(uint8_t value);
  void TIAWrite0x23(uint8_t value);
  void TIAWrite0x24(uint8_t value);
  void TIAWrite0x25(uint8_t value);
  void TIAWrite0x26(uint8_t value);
  void TIAWrite0x27(uint8_t value);
  void TIAWrite0x28(uint8_t value);
  void TIAWrite0x29(uint8_t value);
  void TIAWrite0x2A(uint8_t value);
  void TIAWrite0x2B(uint8_t value);
  void TIAWrite0x2C(uint8_t value);
  void TIAWrite0x2D(uint8_t value);

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