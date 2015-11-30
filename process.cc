#include <node.h>
#include <windows.h>
#include <TlHelp32.h>
#include <vector>
#include "process.h"
#include "memoryjs.h"

process::process() {}

process::~process() {
	CloseHandle(hProcess);
}

using v8::Exception;
using v8::Isolate;
using v8::String;

PROCESSENTRY32 process::processEntry;

PROCESSENTRY32 process::openProcess(const char* processName, Isolate* isolate){
	PROCESSENTRY32 process;

	// A list of processes (PROCESSENTRY32)
	std::vector<PROCESSENTRY32> processes = getProcesses(isolate);

	for(std::vector<PROCESSENTRY32>::size_type i = 0; i != processes.size(); i++){
		// Check to see if this is the process we want.
		if (!strcmp(processes[i].szExeFile, processName)) {
			// Store the process handle and process ID internally
			// for reading/writing to memory
			hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processEntry.th32ProcessID);

			// Store the handle (just for reference to close the handle later)
			// process is returned and processEntry is used internally for reading/writing to memory
			handle = (int) hProcess;
			process = processEntry = processes[i];

			break;
		}
	}

	return process;
}

void process::closeProcess(){
	CloseHandle((HANDLE) handle);
}

std::vector<PROCESSENTRY32> process::getProcesses(Isolate* isolate) {
	// Take a snapshot of all processes.
	HANDLE hProcessSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	PROCESSENTRY32 pEntry;

	std::vector<PROCESSENTRY32> processes;

	if (hProcessSnapshot == INVALID_HANDLE_VALUE) {
		memoryjs::throwError("method failed to take snapshot of the process", isolate);
	}

	// Before use, set the structure size.
	pEntry.dwSize = sizeof(pEntry);

	// Exit if unable to find the first process.
	if (!Process32First(hProcessSnapshot, &pEntry)) {
		CloseHandle(hProcessSnapshot);
		memoryjs::throwError("method failed to retrieve the first process", isolate);
	}

	// Loop through processes.
	do {
		// Add the process to the vector
		processes.push_back(pEntry);
	} while (Process32Next(hProcessSnapshot, &pEntry));

	CloseHandle(hProcessSnapshot);

  return processes;
}
