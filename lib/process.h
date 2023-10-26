#pragma once
#ifndef PROCESS_H
#define PROCESS_H
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <TlHelp32.h>
#include <vector>

// Struct definition copy pasted from Microsoft's documentation.

// For some reason, TiHelp.h doesn't allow you to use the non-unicode version of this struct explicity
// You either explititly use the Unicode version by using PROCESSENTRY32W, or you use PROCESSENTRY32,
// which, if _UNICODE is defined, will resolve to PROCESSENTRY32W.

// Perhaps defining a completely new struct with just the stuff that's needed would be better
struct PROCESSENTRY32A
{
    DWORD   dwSize;
    DWORD   cntUsage;
    DWORD   th32ProcessID;          // this process
    ULONG_PTR th32DefaultHeapID;
    DWORD   th32ModuleID;           // associated exe
    DWORD   cntThreads;
    DWORD   th32ParentProcessID;    // this process's parent process
    LONG    pcPriClassBase;         // Base priority of process's threads
    DWORD   dwFlags;
    CHAR    szExeFile[MAX_PATH];    // Path
} ;

class process {
public:
  struct Pair {
    HANDLE handle;
    PROCESSENTRY32A process;
  };

  process();
  ~process();

  Pair openProcess(const char* processName, char** errorMessage);
  Pair openProcess(DWORD processId, char** errorMessage);
  void closeProcess(HANDLE hProcess);
  std::vector<PROCESSENTRY32A> getProcesses(char** errorMessage);
};

#endif
#pragma once
