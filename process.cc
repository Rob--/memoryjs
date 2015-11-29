#include <node.h>
#include <windows.h>
#include <iostream>
#include <TlHelp32.h>
#include <string>
#include <sstream>
#include "process.h"

process::process() {}

process::~process() {
	CloseHandle(hProcess);
}

using v8::Exception;
using v8::Isolate;
using v8::String;

void throwError(char* error, Isolate* isolate){
  isolate->ThrowException(
    Exception::TypeError(String::NewFromUtf8(isolate, error))
  );
  return;
}

bool process::openProcess(const char* processName, Isolate* isolate) {
  // Take a snapshot of all processes.
	HANDLE hProcessSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	PROCESSENTRY32 pEntry;

	if (hProcessSnapshot == INVALID_HANDLE_VALUE) {
		throwError("openProcess method failed to take snapshot of the process", isolate);
	}

	// Before use, set the structure size.
	pEntry.dwSize = sizeof(pEntry);

	// Exit if unable to find the first process.
	if (!Process32First(hProcessSnapshot, &pEntry)) {
		CloseHandle(hProcessSnapshot);
		throwError("openProcess failed to retrieve the first process", isolate);
	}

	// Loop through processes.
	do {
		// Check to see if this is the process we want.
		if (!strcmp(pEntry.szExeFile, processName)) {
			dwProcessId = pEntry.th32ProcessID;
			CloseHandle(hProcessSnapshot);
			hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwProcessId);
      return TRUE;
		}
	} while (Process32Next(hProcessSnapshot, &pEntry));

  return FALSE;
}
