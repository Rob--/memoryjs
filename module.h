#pragma once
#ifndef MODULE_H
#define MODULE_H
#define WIN32_LEAN_AND_MEAN

#include <node.h>
#include <windows.h>
#include <TlHelp32.h>
#include <vector>

using v8::Isolate;

class module {

public:
	MODULEENTRY32 moduleEntry;

	module();
	~module();

	MODULEENTRY32 findModule(const char* moduleName, DWORD processId, Isolate* isolate);
	std::vector<MODULEENTRY32> getModules(DWORD processId, Isolate* isolate);
};
#endif
#pragma once
