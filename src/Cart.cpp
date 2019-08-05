#include <iostream>
#include <cstring>
#include <fstream>
#include <algorithm>
#include <cstdio>
#include "Cart.h"
#include "Common.h"


Cartridge::Cartridge() {

}

Cartridge::~Cartridge() {
  SafeDeleteArray(m_pROM)
}

void Cartridge::Init() {

}

bool Cartridge::LoadFromFile(const char *path) {
  using namespace std;

  Log("Loading %s...", path);

  Reset();

  strcpy(m_szFilePath, path);

  std::string pathstr(path);
  std::string filename;

  size_t pos = pathstr.find_last_of("\\");
  if (pos != std::string::npos) {
    filename.assign(pathstr.begin() + pos + 1, pathstr.end());
  } else {
    pos = pathstr.find_last_of("/");
    if (pos != std::string::npos) {
      filename.assign(pathstr.begin() + pos + 1, pathstr.end());
    } else {
      filename = pathstr;
    }
  }

  strcpy(m_szFileName, filename.c_str());

  ifstream file(path, ios::in | ios::binary | ios::ate);

  if (file.is_open()) {
    int size = static_cast<int> (file.tellg());
    char *memblock = new char[size];
    file.seekg(0, ios::beg);
    file.read(memblock, size);
    file.close();

    string fn(path);
    transform(fn.begin(), fn.end(), fn.begin(), (int (*)(int)) tolower);

    m_bLoaded = LoadFromBuffer(reinterpret_cast<uint8_t *> (memblock), size);

    if (m_bLoaded) {
      Log("ROM loaded");
    } else {
      Log("There was a problem loading the memory for file %s...", path);
    }

    SafeDeleteArray(memblock);
  } else {
    Log("There was a problem loading the file %s...", path);
    m_bLoaded = false;
  }

  if (!m_bLoaded) {
    Reset();
  }

  return m_bLoaded;
}

bool Cartridge::LoadFromBuffer(const uint8_t *buffer, int size) {
  if (buffer != nullptr) {
    Log("Loading from buffer... Size: %d", size);
    m_iTotalSize = size;
    m_pROM = new uint8_t[m_iTotalSize];
    memcpy(m_pROM, buffer, m_iTotalSize);
    m_bLoaded = true;
    return true;
  } else {
    return false;
  }
}

uint8_t* Cartridge::GetROM() {
  if (m_bLoaded) {
    return m_pROM;
  }

  return m_bLoaded ?  m_pROM : nullptr;
}

void Cartridge::Reset() {

}