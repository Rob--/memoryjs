/**
 * Hardware debugger for memory.js
 * A lot of the hardware debugging code is based on ReClass.NET
 * https://github.com/ReClassNET/ReClass.NET
 */

#include <node.h>
#include <windows.h>
#include <TlHelp32.h>
#include <vector>
#include "debugger.h"
#include "module.h"

bool debugger::attach(DWORD processId, bool killOnDetatch) {
  if (DebugActiveProcess(processId) == 0) {
    return false;
  }
  
  DebugSetProcessKillOnExit(killOnDetatch);
  return true;
}

bool debugger::detatch(DWORD processId) {
  return DebugActiveProcessStop(processId) != 0;
}

bool debugger::setHardwareBreakpoint(DWORD processId, DWORD64 address, Register reg, int trigger, int size) {
  char* errorMessage = "";
  std::vector<THREADENTRY32> threads = module::getThreads(0, &errorMessage);

  if (strcmp(errorMessage, "")) {
    return false;
  }

  for (std::vector<THREADENTRY32>::size_type i = 0; i != threads.size(); i++) {
    if (threads[i].th32OwnerProcessID != processId) {
      continue;
    }

    HANDLE threadHandle = OpenThread(THREAD_SUSPEND_RESUME | THREAD_GET_CONTEXT | THREAD_SET_CONTEXT, false, threads[i].th32ThreadID);

    SuspendThread(threadHandle);

    CONTEXT context = { 0 };
    context.ContextFlags = CONTEXT_DEBUG_REGISTERS;
    GetThreadContext(threadHandle, &context);

    DebugRegister7 dr7;
    dr7.Value = context.Dr7;

    if (reg == Register::DR0) {
      context.Dr0 = address;
      dr7.G0 = true;
      dr7.RW0 = trigger;
      dr7.Len0 = size;
    }

    if (reg == Register::DR1) {
      context.Dr1 = address;
      dr7.G1 = true;
      dr7.RW1 = trigger;
      dr7.Len1 = size;
    }

    if (reg == Register::DR2) {
      context.Dr2 = address;
      dr7.G2 = true;
      dr7.RW2 = trigger;
      dr7.Len2 = size;
    }

    if (reg == Register::DR3) {
      context.Dr3 = address;
      dr7.G3 = true;
      dr7.RW3 = trigger;
      dr7.Len3 = size;
    }

    context.Dr7 = dr7.Value;

    SetThreadContext(threadHandle, &context);
    ResumeThread(threadHandle);
    CloseHandle(threadHandle);

    return true;
  }

  return false;
}

bool debugger::awaitDebugEvent(DWORD millisTimeout, DebugEvent *info) {
  DEBUG_EVENT debugEvent = {};

  if (WaitForDebugEvent(&debugEvent, millisTimeout) == 0) {
    return false;
  }

  if (debugEvent.dwDebugEventCode == CREATE_PROCESS_DEBUG_EVENT) {
    CloseHandle(debugEvent.u.CreateProcessInfo.hFile);
  }

  if (debugEvent.dwDebugEventCode == LOAD_DLL_DEBUG_EVENT) {
    CloseHandle(debugEvent.u.LoadDll.hFile);
  }

  if (debugEvent.dwDebugEventCode == EXCEPTION_DEBUG_EVENT) {
    EXCEPTION_DEBUG_INFO exception = debugEvent.u.Exception;

    info->processId = debugEvent.dwProcessId;
    info->threadId = debugEvent.dwThreadId;
    info->exceptionAddress = exception.ExceptionRecord.ExceptionAddress;
    info->exceptionCode = exception.ExceptionRecord.ExceptionCode;
    info->exceptionFlags = exception.ExceptionRecord.ExceptionFlags;

    HANDLE handle = OpenThread(THREAD_GET_CONTEXT, false, debugEvent.dwThreadId);
    
    CONTEXT context = {};
    context.ContextFlags = CONTEXT_CONTROL | CONTEXT_INTEGER | CONTEXT_DEBUG_REGISTERS;
    GetThreadContext(handle, &context);

    DebugRegister6 dr6;
    dr6.Value = context.Dr6;

    if (dr6.DR0) {
      info->hardwareRegister = Register::DR0;
    }

    if (dr6.DR1) {
      info->hardwareRegister = Register::DR1;
    }

    if (dr6.DR2) {
      info->hardwareRegister = Register::DR2;
    }

    if (dr6.DR3) {
      info->hardwareRegister = Register::DR3;
    }

    if (!dr6.DR0 && !dr6.DR1 && !dr6.DR2 && !dr6.DR3) {
      info->hardwareRegister = Register::Invalid;
    }

    CloseHandle(handle);
  } else {
    ContinueDebugEvent(debugEvent.dwProcessId, debugEvent.dwThreadId, DBG_CONTINUE);
  }

  return true;
}

bool debugger::handleDebugEvent(DWORD processId, DWORD threadId) {
  return ContinueDebugEvent(processId, threadId, DBG_CONTINUE);
  // if (status == DebugContinueStatus::Handled) {
  //   return ContinueDebugEvent(processId, threadId, DBG_CONTINUE) != 0;
  // }

  // if (status == DebugContinueStatus::NotHandled) {
  //   return ContinueDebugEvent(processId, threadId, DBG_EXCEPTION_NOT_HANDLED) != 0;
  // }
}