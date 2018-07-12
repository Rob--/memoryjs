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
HANDLE process::hProcess;

PROCESSENTRY32 process::openProcess(const char* processName, char** errorMessage){
  PROCESSENTRY32 process;

  // A list of processes (PROCESSENTRY32)
  std::vector<PROCESSENTRY32> processes = getProcesses(errorMessage);

  for (std::vector<PROCESSENTRY32>::size_type i = 0; i != processes.size(); i++) {
    // Check to see if this is the process we want.
    if (!strcmp(processes[i].szExeFile, processName)) {
      // Store the process handle and process ID internally
      // for reading/writing to memory
      process::hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processes[i].th32ProcessID);

      // Store the handle (just for reference to close the handle later)
      // process is returned and processEntry is used internally for reading/writing to memory
      handle = (int) process::hProcess;
      process = processEntry = processes[i];

      break;
    }
  }

  if (hProcess == NULL) {
    *errorMessage = "unable to find process";
  }

  return process;
}

PROCESSENTRY32 process::openProcess(DWORD processId, char** errorMessage) {
  PROCESSENTRY32 process;

  // A list of processes (PROCESSENTRY32)
  std::vector<PROCESSENTRY32> processes = getProcesses(errorMessage);

  for (std::vector<PROCESSENTRY32>::size_type i = 0; i != processes.size(); i++) {
    // Check to see if this is the process we want.
    if (processId == processes[i].th32ProcessID) {
      // Store the process handle and process ID internally
      // for reading/writing to memory
      process::hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processes[i].th32ProcessID);

      // Store the handle (just for reference to close the handle later)
      // process is returned and processEntry is used internally for reading/writing to memory
      handle = (int) process::hProcess;
      process = processEntry = processes[i];

      break;
    }
  }

  if (hProcess == NULL) {
    *errorMessage = "unable to find process";
  }

  return process;
}

void process::closeProcess(){
  CloseHandle(process::hProcess);
}

std::vector<PROCESSENTRY32> process::getProcesses(char** errorMessage) {
  // Take a snapshot of all processes.
  HANDLE hProcessSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
  PROCESSENTRY32 pEntry;

  if (hProcessSnapshot == INVALID_HANDLE_VALUE) {
    *errorMessage = "method failed to take snapshot of the process";
  }

  // Before use, set the structure size.
  pEntry.dwSize = sizeof(pEntry);

  // Exit if unable to find the first process.
  if (!Process32First(hProcessSnapshot, &pEntry)) {
    CloseHandle(hProcessSnapshot);
    *errorMessage = "method failed to retrieve the first process";
  }

  std::vector<PROCESSENTRY32> processes;

  // Loop through processes.
  do {
    // Add the process to the vector
    processes.push_back(pEntry);
  } while (Process32Next(hProcessSnapshot, &pEntry));

  CloseHandle(hProcessSnapshot);
  return processes;
}
