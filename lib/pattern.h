#pragma once
#ifndef PATTERN_H
#define PATTERN_H
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <TlHelp32.h>

class pattern {
public:
  pattern();
  ~pattern();

  // Signature/pattern types
  enum {
    // normal: normal
    // read: read memory at pattern
    // subtract: subtract module base
    ST_NORMAL = 0x0,
    ST_READ = 0x1,
    ST_SUBTRACT = 0x2
  };

  bool search(HANDLE handle, std::vector<MEMORY_BASIC_INFORMATION> regions, DWORD64 searchAddress, const char* pattern, short flags, uint32_t patternOffset, uintptr_t* pAddress);
  bool search(HANDLE handle, std::vector<MODULEENTRY32> modules, DWORD64 searchAddress, const char* pattern, short flags, uint32_t patternOffset, uintptr_t* pAddress);
  bool findPattern(HANDLE handle, uintptr_t memoryBase, unsigned char* module, DWORD memorySize, const char* pattern, short flags, uint32_t patternOffset, uintptr_t* pAddress);
  bool compareBytes(const unsigned char* bytes, const char* pattern);
};

#endif
#pragma once
