#include <node.h>
#include <windows.h>
#include <TlHelp32.h>
#include <vector>
#include "pattern.h"
#include "memoryjs.h"
#include "process.h"
#include "memory.h"

#define INRANGE(x,a,b) (x >= a && x <= b) 
#define getBits( x ) (INRANGE(x,'0','9') ? (x - '0') : ((x&(~0x20)) - 'A' + 0xa))
#define getByte( x ) (getBits(x[0]) << 4 | getBits(x[1]))

pattern::pattern() {}
pattern::~pattern() {}

bool pattern::search(HANDLE handle, std::vector<MEMORY_BASIC_INFORMATION> regions, DWORD64 searchAddress, const char* pattern, short flags, uint32_t patternOffset, uintptr_t* pAddress) {
  for (std::vector<MEMORY_BASIC_INFORMATION>::size_type i = 0; i != regions.size(); i++) {
    uintptr_t baseAddress = (uintptr_t) regions[i].BaseAddress;
    DWORD baseSize = regions[i].RegionSize;

    // if `searchAddress` has been set, only pattern match if the address lies inside of this region
    if (searchAddress != 0 && (searchAddress < baseAddress || searchAddress > (baseAddress + baseSize))) {
      continue;
    }

    // read memory region to pattern match inside
    std::vector<unsigned char> regionBytes = std::vector<unsigned char>(baseSize);
    ReadProcessMemory(handle, (LPVOID)baseAddress, &regionBytes[0], baseSize, nullptr);
    unsigned char* byteBase = const_cast<unsigned char*>(&regionBytes.at(0));

    if (findPattern(handle, baseAddress, byteBase, baseSize, pattern, flags, patternOffset, pAddress)) {
      return true;
    }
  }

  return false;
}

bool pattern::search(HANDLE handle, std::vector<MODULEENTRY32> modules, DWORD64 searchAddress, const char* pattern, short flags, uint32_t patternOffset, uintptr_t* pAddress) {
  for (std::vector<MODULEENTRY32>::size_type i = 0; i != modules.size(); i++) {
    uintptr_t baseAddress = (uintptr_t) modules[i].modBaseAddr;
    DWORD baseSize = modules[i].modBaseSize;

    // if `searchAddress` has been set, only pattern match if the address lies inside of this module
    if (searchAddress != 0 && (searchAddress < baseAddress || searchAddress > (baseAddress + baseSize))) {
      continue;
    }

    // read memory region occupied by the module to pattern match inside
    std::vector<unsigned char> moduleBytes = std::vector<unsigned char>(baseSize);
    ReadProcessMemory(handle, (LPVOID)baseAddress, &moduleBytes[0], baseSize, nullptr);
    unsigned char* byteBase = const_cast<unsigned char*>(&moduleBytes.at(0));

    if (findPattern(handle, baseAddress, byteBase, baseSize, pattern, flags, patternOffset, pAddress)) {
      return true;
    }
  }

  return false;
}

/* based off Y3t1y3t's implementation */
bool pattern::findPattern(HANDLE handle, uintptr_t memoryBase, unsigned char* byteBase, DWORD memorySize, const char* pattern, short flags, uint32_t patternOffset, uintptr_t* pAddress) {
  // uintptr_t moduleBase = (uintptr_t)module.hModule;
  auto maxOffset = memorySize - 0x1000;

  for (uintptr_t offset = 0; offset < maxOffset; ++offset) {
    if (compareBytes(byteBase + offset, pattern)) {
      uintptr_t address = memoryBase + offset + patternOffset;

      if (flags & ST_READ) {
        ReadProcessMemory(handle, LPCVOID(address), &address, sizeof(uintptr_t), nullptr);
      }

      if (flags & ST_SUBTRACT) {
        address -= memoryBase;
      }

      *pAddress = address;

      return true;
    }
  }

  return false;
};

bool pattern::compareBytes(const unsigned char* bytes, const char* pattern) {
  for (; *pattern; *pattern != ' ' ? ++bytes : bytes, ++pattern) {
    if (*pattern == ' ' || *pattern == '?') {
      continue;
    }
		
    if (*bytes != getByte(pattern)) {
      return false;
    }
    
    ++pattern;
  }
  
  return true;
}