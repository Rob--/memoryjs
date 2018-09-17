#pragma once
#ifndef MEMORY_H
#define MEMORY_H
#define WIN32_LEAN_AND_MEAN

#include <node.h>
#include <windows.h>
#include <TlHelp32.h>

using v8::Isolate;

class memory {

public:
  template <class dataType>
  dataType readMemory(HANDLE hProcess, DWORD64 address) {
    dataType cRead;
    ReadProcessMemory(hProcess, (LPVOID)address, &cRead, sizeof(dataType), NULL);
    return cRead;
  }

  char* readBuffer(HANDLE hProcess, DWORD64 address, SIZE_T size) {
    char* buffer = new char[size];
    ReadProcessMemory(hProcess, (LPVOID)address, buffer, size, NULL);
    return buffer;
  }

  char readChar(HANDLE hProcess, DWORD64 address) {
    char value;
    ReadProcessMemory(hProcess, (LPVOID)address, &value, sizeof(char), NULL);
    return value;
	}

  template <class dataType>
  void writeMemory(HANDLE hProcess, DWORD64 address, dataType value) {
    WriteProcessMemory(hProcess, (LPVOID)address, &value, sizeof(dataType), NULL);
  }

  template <class dataType>
  void writeMemory(HANDLE hProcess, DWORD64 address, dataType value, SIZE_T size) {
	  LPVOID buffer = value;

	  if (typeid(dataType) != typeid(char*)) {
		  buffer = &value;
	  }

	  WriteProcessMemory(hProcess, (LPVOID)address, buffer, size, NULL);
  }

  // Write String, Method 1: Utf8Value is converted to string, get pointer and length from string
  // template <>
  // void writeMemory<std::string>(HANDLE hProcess, DWORD address, std::string value) {
  //  WriteProcessMemory(hProcess, (LPVOID)address, value.c_str(), value.length(), NULL);
  // }

  // Write String, Method 2: get pointer and length from Utf8Value directly
  void writeMemory(HANDLE hProcess, DWORD64 address, char* value, SIZE_T size) {
    WriteProcessMemory(hProcess, (LPVOID)address, value, size, NULL);
  }

  // Set the protection of the memory, returns previous protection.
  void setProtection(HANDLE hProcess, DWORD64 address, SIZE_T size, DWORD dwProtection, PDWORD dwOldProtection) {
	  VirtualProtectEx(hProcess, (LPVOID)address, size, dwProtection, dwOldProtection);
  }
};
#endif
#pragma once