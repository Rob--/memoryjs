#pragma once
#ifndef PROCESS_H
#define PROCESS_H
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <TlHelp32.h>
#include <vector>

class process {
public:
  struct Pair {
    HANDLE handle;
    PROCESSENTRY32 process;
  };

  process();
  ~process();

  Pair openProcess(const char* processName, const char** errorMessage);
  Pair openProcess(DWORD processId, const char** errorMessage);
  void closeProcess(HANDLE hProcess);
  std::vector<PROCESSENTRY32> getProcesses(const char** errorMessage);
};

#endif
#pragma once
