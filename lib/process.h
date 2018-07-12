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
  static HANDLE hProcess;

  static PROCESSENTRY32 processEntry;
  int handle = 0;

  process();
  ~process();

  PROCESSENTRY32 openProcess(const char* processName, char** errorMessage);
  PROCESSENTRY32 openProcess(DWORD processId, char** errorMessage);
  void closeProcess();
  std::vector<PROCESSENTRY32> getProcesses(char** errorMessage);
};
#endif
#pragma once
