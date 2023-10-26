#include <node.h>
#include <windows.h>
#include <TlHelp32.h>
#include <vector>
#include "process.h"
#include "memoryjs.h"

process::process() {}
process::~process() {}

using v8::Exception;
using v8::Isolate;
using v8::String;

process::Pair process::openProcess(const char* processName, char** errorMessage){
  PROCESSENTRY32A process;
  HANDLE handle = NULL;

  // A list of processes (PROCESSENTRY32A)
  std::vector<PROCESSENTRY32A> processes = getProcesses(errorMessage);

  for (std::vector<PROCESSENTRY32A>::size_type i = 0; i != processes.size(); i++) {
    // Check to see if this is the process we want.
    if (!strcmp(processes[i].szExeFile, processName)) {
      handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processes[i].th32ProcessID);
      process = processes[i];
      break;
    }
  }

  if (handle == NULL) {
    *errorMessage = "unable to find process";
  }

  return {
    handle,
    process,
  };
}

process::Pair process::openProcess(DWORD processId, char** errorMessage) {
  PROCESSENTRY32A process;
  HANDLE handle = NULL;

  // A list of processes (PROCESSENTRY32A)
  std::vector<PROCESSENTRY32A> processes = getProcesses(errorMessage);

  for (std::vector<PROCESSENTRY32A>::size_type i = 0; i != processes.size(); i++) {
    // Check to see if this is the process we want.
    if (processId == processes[i].th32ProcessID) {
      handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processes[i].th32ProcessID);
      process = processes[i];
      break;
    }
  }

  if (handle == NULL) {
    *errorMessage = "unable to find process";
  }

  return {
    handle,
    process,
  };
}

std::vector<PROCESSENTRY32A> process::getProcesses(char** errorMessage) {
  // Take a snapshot of all processes.
  HANDLE hProcessSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
  PROCESSENTRY32W pEntry;

  if (hProcessSnapshot == INVALID_HANDLE_VALUE) {
    *errorMessage = "method failed to take snapshot of the process";
  }

  // Before use, set the structure size.
  pEntry.dwSize = sizeof(pEntry);

  // Exit if unable to find the first process.
  if (!Process32FirstW(hProcessSnapshot, &pEntry)) {
    CloseHandle(hProcessSnapshot);
    *errorMessage = "method failed to retrieve the first process";
  }

  std::vector<PROCESSENTRY32A> processes;

  // Loop through processes.
  do {
    // Add the process to the vector
    PROCESSENTRY32A pEntryReal;
    pEntryReal.dwSize = sizeof(pEntryReal);
    
    pEntryReal.cntUsage = pEntry.cntUsage;
    pEntryReal.th32ProcessID = pEntry.th32ProcessID;        
    pEntryReal.th32DefaultHeapID = pEntry.th32DefaultHeapID;
    pEntryReal.th32ModuleID = pEntry.th32ModuleID;         
    pEntryReal.cntThreads = pEntry.cntThreads;
    pEntryReal.th32ParentProcessID = pEntry.th32ParentProcessID;  
    pEntryReal.pcPriClassBase = pEntry.pcPriClassBase;       
    pEntryReal.dwFlags = pEntry.dwFlags;

    // We use UTF-8 strings everywhere else in the program, but because the Windows API only supports Unicode through UTF-16,
    // we have to convert the UTF-16 strings that Windows gives us to UTF-8 for use in the rest of the program
    WideCharToMultiByte(CP_UTF8, 0, pEntry.szExeFile, -1, pEntryReal.szExeFile, sizeof(pEntryReal.szExeFile), NULL, NULL);

    processes.push_back(pEntryReal);
  } while (Process32NextW(hProcessSnapshot, &pEntry));

  CloseHandle(hProcessSnapshot);
  return processes;
}
