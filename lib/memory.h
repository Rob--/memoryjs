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
  dataType readMemory(HANDLE hProcess, DWORD64 dwAddress) {
    dataType cRead;
    ReadProcessMemory(hProcess, (LPVOID)dwAddress, &cRead, sizeof(dataType), NULL);
    return cRead;
  }

  char* readMemoryString(HANDLE hProcess, DWORD64 dwAddress, SIZE_T size) {
    char* value = new char[size + 1];
    ReadProcessMemory(hProcess, (LPVOID)dwAddress, value, size, NULL);
    return value;
  }

  char readMemoryChar(HANDLE hProcess, DWORD64 dwAddress) {
    char value;
    ReadProcessMemory(hProcess, (LPVOID)dwAddress, &value, sizeof(char), NULL);
    return value;
	}

  template <class dataType>
  void writeMemory(HANDLE hProcess, DWORD64 dwAddress, dataType value) {
    WriteProcessMemory(hProcess, (LPVOID)dwAddress, &value, sizeof(dataType), NULL);
  }

  // Write String, Method 1: Utf8Value is converted to string, get pointer and length from string
  // template <>
  // void writeMemory<std::string>(HANDLE hProcess, DWORD dwAddress, std::string value) {
  //  WriteProcessMemory(hProcess, (LPVOID)dwAddress, value.c_str(), value.length(), NULL);
  // }

  // Write String, Method 2: get pointer and length from Utf8Value directly
  void writeMemory(HANDLE hProcess, DWORD64 dwAddress, char* value, SIZE_T size) {
    WriteProcessMemory(hProcess, (LPVOID)dwAddress, value, size, NULL);
  }

  // Set the protection of the memory, returns previous protection.
  void setProtection(HANDLE hProcess, DWORD64 dwAddress, SIZE_T size, DWORD dwProtection, PDWORD dwOldProtection) {
	  VirtualProtectEx(hProcess, (LPVOID)dwAddress, size, dwProtection, dwOldProtection);
  }
};
#endif
#pragma once