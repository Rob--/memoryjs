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

	template <class cData>
	cData readMemory(HANDLE hProcess, DWORD dwAddress) {
		cData cRead;
		ReadProcessMemory(hProcess, (LPVOID)dwAddress, &cRead, sizeof(cData), NULL);
		return cRead;
	}

	template <class cData>
	void writeMemory(HANDLE hProcess, DWORD dwAddress, cData value) {
		WriteProcessMemory(hProcess, (LPVOID)dwAddress, &value, sizeof(cData), NULL);
	}
};
#endif
#pragma once