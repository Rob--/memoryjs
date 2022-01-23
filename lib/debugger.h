#pragma once
#ifndef debugger_H
#define debugger_H
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <TlHelp32.h>
#include <vector>

enum class Register {
  Invalid = -0x1,
  DR0 = 0x0,
  DR1 = 0x1,
  DR2 = 0x2,
  DR3 = 0x3
};

struct DebugRegister6 {
	union {
		uintptr_t Value;
		struct {
			unsigned DR0 : 1;
			unsigned DR1 : 1;
			unsigned DR2 : 1;
			unsigned DR3 : 1;
			unsigned Reserved : 9;
			unsigned BD : 1;
			unsigned BS : 1;
			unsigned BT : 1;
		};
	};
};

struct DebugRegister7 {
	union {
		uintptr_t Value;
		struct {
			unsigned G0 : 1;
			unsigned L0 : 1;
			unsigned G1 : 1;
			unsigned L1 : 1;
			unsigned G2 : 1;
			unsigned L2 : 1;
			unsigned G3 : 1;
			unsigned L3 : 1;
			unsigned GE : 1;
			unsigned LE : 1;
			unsigned Reserved : 6;
			unsigned RW0 : 2;
			unsigned Len0 : 2;
			unsigned RW1 : 2;
			unsigned Len1 : 2;
			unsigned RW2 : 2;
			unsigned Len2 : 2;
			unsigned RW3 : 2;
			unsigned Len3 : 2;
		};
	};
};

struct DebugEvent {
  DWORD processId;
  DWORD threadId;
  DWORD exceptionCode;
  DWORD exceptionFlags;
  void* exceptionAddress;
  Register hardwareRegister;
};

namespace debugger {
  bool attach(DWORD processId, bool killOnDetatch);
  bool detatch(DWORD processId);
  bool setHardwareBreakpoint(DWORD processId, DWORD64 address, Register reg, int trigger, int size);
  bool awaitDebugEvent(DWORD millisTimeout, DebugEvent *info);
  bool handleDebugEvent(DWORD processId, DWORD threadId);
}

#endif
#pragma once
