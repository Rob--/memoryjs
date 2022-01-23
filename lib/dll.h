#pragma once
#ifndef DLL_H
#define DLL_H
#define WIN32_LEAN_AND_MEAN

#include <napi.h>
#include <windows.h>
#include <TlHelp32.h>
#include <string>

namespace dll {
  bool inject(HANDLE handle, std::string dllPath, char** errorMessage, LPDWORD moduleHandle) {
    // allocate space in target process memory for DLL path
    LPVOID targetProcessPath = VirtualAllocEx(handle, NULL, dllPath.length() + 1, MEM_COMMIT, PAGE_EXECUTE_READWRITE);

    if (targetProcessPath == NULL) {
      *errorMessage = "unable to allocate memory in target process";
      return false;
    }

    // write DLL path to reserved memory space
    if (WriteProcessMemory(handle, targetProcessPath, dllPath.c_str(), dllPath.length() + 1, 0) == 0) {
      *errorMessage = "unable to to write dll path to target process";
      VirtualFreeEx(handle, targetProcessPath, 0, MEM_RELEASE);
      return false;
    }

    HMODULE kernel32 = LoadLibrary("kernel32");

    if (kernel32 == 0) {
      VirtualFreeEx(handle, targetProcessPath, 0, MEM_RELEASE);
      *errorMessage = "unable to load kernel32";
      return false;
    }

    // call LoadLibrary from target process
    LPTHREAD_START_ROUTINE loadLibraryAddress = (LPTHREAD_START_ROUTINE) GetProcAddress(kernel32, "LoadLibraryA");
    HANDLE thread = CreateRemoteThread(handle, NULL, NULL, loadLibraryAddress, targetProcessPath, NULL, NULL);

    if (thread == NULL) {
      *errorMessage = "unable to call LoadLibrary from target process";
      VirtualFreeEx(handle, targetProcessPath, 0, MEM_RELEASE);
      return false;
    }

    WaitForSingleObject(thread, INFINITE);
    GetExitCodeThread(thread, moduleHandle);

    // free memory reserved in target process
    VirtualFreeEx(handle, targetProcessPath, 0, MEM_RELEASE);
    CloseHandle(thread);

    return *moduleHandle > 0;
  }

  bool unload(HANDLE handle, char** errorMessage, HMODULE moduleHandle) {
    HMODULE kernel32 = LoadLibrary("kernel32");

    if (kernel32 == 0) {
      *errorMessage = "unable to load kernel32";
      return false;
    }

    // call FreeLibrary from target process
    LPTHREAD_START_ROUTINE freeLibraryAddress = (LPTHREAD_START_ROUTINE) GetProcAddress(kernel32, "FreeLibrary");
    HANDLE thread = CreateRemoteThread(handle, NULL, NULL, freeLibraryAddress, (void*)moduleHandle, NULL, NULL);

    if (thread == NULL) {
      *errorMessage = "unable to call FreeLibrary from target process";
      return false;
    }

    WaitForSingleObject(thread, INFINITE);
    DWORD exitCode = -1;
    GetExitCodeThread(thread, &exitCode);
    CloseHandle(thread);

    return exitCode != 0;
  }
}

#endif
#pragma once
