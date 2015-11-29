#pragma once
#ifndef PROCESS_H
#define PROCESS_H
#define WIN32_LEAN_AND_MEAN

#include <node.h>
#include <windows.h>

using v8::Isolate;

class process {

public:
	HANDLE hProcess;
	DWORD dwProcessId, dwClient;
	HWND tWindow;

	process();
	~process();

	bool openProcess(const char* name, Isolate* isolate);
};
#endif
#pragma once
