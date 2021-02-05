#pragma once

#include <list>
#include <cstdint>

class Cartridge
{
public:
  void Reset() {};
  bool LoadFromFile(const char* path);
  bool LoadFromBuffer(const uint8_t* buffer, int size);

  uint8_t* GetROM();
  uint16_t GetRomSize();

private:
  std::vector<uint8_t> m_ROM;
  int m_iTotalSize;
  bool m_bLoaded;
  char m_szFilePath[512];
  char m_szFileName[512];
};
