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
	dataType readMemory(HANDLE hProcess, DWORD dwAddress) {
		dataType cRead;
		ReadProcessMemory(hProcess, (LPVOID)dwAddress, &cRead, sizeof(dataType), NULL);
		return cRead;
	}

	template <class dataType>
	void writeMemory(HANDLE hProcess, DWORD dwAddress, dataType value) {
		WriteProcessMemory(hProcess, (LPVOID)dwAddress, &value, sizeof(dataType), NULL);
	}
};
#endif
#pragma once