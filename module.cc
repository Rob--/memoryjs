#include <node.h>
#include <windows.h>
#include <TlHelp32.h>
#include <vector>
#include "module.h"
#include "process.h"
#include "memoryjs.h"

module::module() {}

module::~module() {}

using v8::Exception;
using v8::Isolate;
using v8::String;

MODULEENTRY32 module::findModule(const char* moduleName, DWORD processId, Isolate* isolate) {
	MODULEENTRY32 module;

	// A list of modules (MODULEENTRY32)
	std::vector<MODULEENTRY32> modules = getModules(processId, isolate);

	// Loop over every module
	for (std::vector<MODULEENTRY32>::size_type i = 0; i != modules.size(); i++) {
		// Check to see if this is the module we want.
		if (!strcmp(modules[i].szModule, moduleName)) {
			// module is returned and moduleEntry is used internally for reading/writing to memory
			module = moduleEntry = modules[i];

			break;
		}
	}

	return module;
}

std::vector<MODULEENTRY32> module::getModules(DWORD processId, Isolate* isolate) {	
	// Take a snapshot of all modules inside a given process.
	HANDLE hModuleSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, processId);
	MODULEENTRY32 mEntry;

	std::vector<MODULEENTRY32> modules;

	if (hModuleSnapshot == INVALID_HANDLE_VALUE) {
		memoryjs::throwError("method failed to take snapshot of the process", isolate);
	}

	// Before use, set the structure size.
	mEntry.dwSize = sizeof(mEntry);

	// Exit if unable to find the first module.
	if (!Module32First(hModuleSnapshot, &mEntry)) {
		CloseHandle(hModuleSnapshot);
		memoryjs::throwError("method failed to retrieve the first module", isolate);
	}

	// Loop through modules.
	do {
		// Add the module to the vector
		modules.push_back(mEntry);
	} while (Module32Next(hModuleSnapshot, &mEntry));

	return modules;
}