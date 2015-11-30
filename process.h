#pragma once
#ifndef PROCESS_H
#define PROCESS_H
#define WIN32_LEAN_AND_MEAN

#include <node.h>
#include <windows.h>
#include <TlHelp32.h>
#include <vector>

using v8::Isolate;

class process {

public:
	HANDLE hProcess;

	static PROCESSENTRY32 processEntry;
	int handle = 0;

	process();
	~process();

	PROCESSENTRY32 openProcess(const char* processName, Isolate* isolate);
	void closeProcess();
	std::vector<PROCESSENTRY32> getProcesses(Isolate* isolate);

};
#endif
#pragma once
