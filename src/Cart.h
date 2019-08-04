#ifndef ATARI_VCS_EMU_CART_H
#define ATARI_VCS_EMU_CART_H

#include <list>
#include <cstdint>

class Cartridge
{
public:
  Cartridge();
  ~Cartridge();
  void Init();
  void Reset();
  bool LoadFromFile(const char* path);
  bool LoadFromBuffer(const uint8_t* buffer, int size);

private:
  bool GatherMetadata();

private:
  uint8_t* m_pTheROM;
  int m_iTotalSize;
  bool m_bLoaded;
  char m_szFilePath[512];
  char m_szFileName[512];
};

#endif //ATARI_VCS_EMU_CART_H
