# memoryjs &middot; [![GitHub license](https://img.shields.io/github/license/Rob--/memoryjs)](https://github.com/Rob--/memoryjs/blob/master/LICENSE.md) [![npm version](https://img.shields.io/npm/v/memoryjs.svg?style=flat)](https://www.npmjs.com/package/memoryjs) ![npm](https://img.shields.io/npm/dy/memoryjs)

memoryjs is an NPM package for reading and writing process memory! (finally!)

NOTE: version 3 of this library introduces breaking changes that are incompatible with previous versions.
The notable change is that when reading memory, writing memory and pattern scanning you are required to pass the handle
through for the process (that is returned from `memoryjs.openProcess`). This allows for multi-process support.

# Features

- List all open processes
- List all modules associated with a process
- Find a specific module within a process
- Read process memory
- Write process memory
- Read buffers from memory
- Write buffer to memory
- Change memory protection
- Reserve/allocate, commit or change regions of memory
- Fetch a list of memory regions within a process
- Pattern scanning
- Execute a function within a process
- Hardware breakpoints (find out what accesses/writes to this address etc)
- Inject DLLs
- Unload DLLs

Functions that this library directly exposes from the WinAPI:
- [ReadProcessMemory](https://docs.microsoft.com/en-us/windows/desktop/api/memoryapi/nf-memoryapi-readprocessmemory)
- [WriteProcessMemory](https://docs.microsoft.com/en-us/windows/desktop/api/memoryapi/nf-memoryapi-writeprocessmemory)
- [VirtualProtectEx](https://docs.microsoft.com/en-us/windows/desktop/api/memoryapi/nf-memoryapi-virtualprotectex)
- [VirtualAllocEx](https://docs.microsoft.com/en-us/windows/desktop/api/memoryapi/nf-memoryapi-virtualallocex)

TODO:
- WriteFile support (for driver interactions)

# Install

This is a Node add-on (last tested to be working on `v14.15.0`) and therefore requires [node-gyp](https://github.com/nodejs/node-gyp) to use.

You may also need to [follow these steps](https://github.com/nodejs/node-gyp#user-content-installation).

`npm install memoryjs`

When using memoryjs, the target process should match the platform architecture of the Node version running.
For example if you want to target a 64 bit process, you should try and use a 64 bit version of Node.

You also need to recompile the library and target the platform you want. Head to the memoryjs node module directory, open up a terminal and to run the compile scripts, type one of the following:

```bash
# will automatically compile based on the detected Node architecture
npm run build

# compile to target 32 bit processes
npm run build32

# compile to target 64 bit processes
npm run build64
```

# Node Webkit / Electron

If you are planning to use this module with Node Webkit or Electron, take a look at [Liam Mitchell](https://github.com/LiamKarlMitchell)'s build notes [here](https://github.com/Rob--/memoryjs/issues/23).

# Usage

Initialise:
``` javascript
const memoryjs = require('memoryjs');
const processName = "csgo.exe";
```

### Processes:

Open a process (sync):
``` javascript
const processObject = memoryjs.openProcess(processIdentifier);
```

Open a process (async):
``` javascript
memoryjs.openProcess(processIdentifier, (error, processObject) => {

});
```

Close/release process handle:
``` javascript
memoryjs.closeProcess(handle);
```

Get all processes (sync):
``` javascript
const processes = memoryjs.getProcesses();
```

Get all processes (async):
``` javascript
memoryjs.getProcesses((error, processes) => {

});
```

See the [Documentation](#user-content-process-object) section of this README to see what a process object looks like.

### Modules: 

Find a module (sync):
``` javascript
const module = memoryjs.findModule(moduleName, processId);
```

Find a module (async):
``` javascript
memoryjs.findModule(moduleName, processId, (error, module) => {

});
```

Get all modules (sync):
``` javascript
const modules = memoryjs.getModules(processId);
```

Get all modules (async):
``` javascript
memoryjs.getModules(processId, (error, modules) => {

});
```

See the [Documentation](#user-content-module-object) section of this README to see what a module object looks like.

### Memory:

Read from memory (sync):
``` javascript
const value = memoryjs.readMemory(handle, address, dataType);
```

Read from memory (async):
``` javascript
memoryjs.readMemory(handle, address, dataType, (error, value) => {

});
```

Read buffer from memory (sync):
``` javascript
const buffer = memoryjs.readBuffer(handle, address, size);
```

Read buffer from memory (async):
``` javascript
memoryjs.readBuffer(handle, address, size, (error, buffer) => {

});
```

Write to memory:
``` javascript
memoryjs.writeMemory(handle, address, value, dataType);
```

Write buffer to memory:
``` javascript
memoryjs.writeBuffer(handle, address, buffer);
```

Fetch memory regions (sync):
``` javascript
const regions = memoryjs.getRegions(handle);
```

Fetch memory regions (async):
``` javascript
memoryjs.getRegions(handle, (regions) => {

});
```

See the [Documentation](#user-content-documentation) section of this README to see what values `dataType` can be.

### Protection:

Set protection of memory:
``` javascript
const oldProtection = memoryjs.virtualProtectEx(handle, address, size, protection);
```

See the [Documentation](#user-content-protection-type) section of this README to see what values `protection` can be.


### Pattern Scanning:

Pattern scanning (sync):
``` javascript
const address = memoryjs.findPattern(handle, pattern, flags, patternOffset);
const address = memoryjs.findPattern(handle, moduleName, pattern, flags, patternOffset);
const address = memoryjs.findPattern(handle, baseAddress, pattern, flags, patternOffset);
```

Pattern scanning (async):
``` javascript
memoryjs.findPattern(handle, pattern, flags, patternOffset, (error, address) => {});
memoryjs.findPattern(handle, moduleName, pattern, flags, patternOffset, (error, address) => {});
memoryjs.findPattern(handle, baseAddress, pattern, flags, patternOffset, (error, address) => {});
```

### Function Execution:

Function execution (sync):
``` javascript
const result = memoryjs.callFunction(handle, args, returnType, address);
```

Function execution (async):
``` javascript
memoryjs.callFunction(handle, args, returnType, address, (error, result) => {

});
```

### DLL Injection:

Inject DLL (sync):
```javascript
const success = memoryjs.injectDll(handle, dllPath);
```

Inject DLL (async):
```javascript
memoryjs.injectDll(handle, dllPath, (error, success) => {

});
```

Unload DLL (sync):
```javascript
const success = memoryjs.unloadDll(handle, moduleBaseAddress);
const success = memoryjs.unloadDll(handle, moduleName);
```

Unload DLL (async):
```javascript
memoryjs.unloadDll(handle, moduleBaseAddress, (error, success) => {

});
memoryjs.unloadDll(handle, moduleName, (error, success) => {

});
```

Click [here](#user-content-result-object) to see what a result object looks like.
Click [here](#user-content-function-execution-1) for details about how to format the arguments and the return type.

### Hardware Breakpoints

Attach a debugger:
``` javascript
const success = memoryjs.attatchDebugger(processId, exitOnDetatch);
```

Detatch debugger:
``` javascript
const success = memoryjs.detatchDebugger(processId);
```

Wait for debug devent:
``` javascript
const success = memoryjs.awaitDebugEvent(hardwareRegister, millisTimeout);
```

Handle debug event:
``` javascript
const success = memoryjs.handleDebugEvent(processId, threadId);
```

Set a hardware breakpoint:
``` javascript
const success = memoryjs.setHardwareBreakpoint(processId, address, hardwareRegister, trigger, length);
```

Remove a hardware breakpoint:
``` javascript
const success = memoryjs.removeHardwareBreakpoint(processId, hardwareRegister);
```

# Documentation

Note: this documentation is currently being updated, refer to the [Wiki](https://github.com/Rob--/memoryjs/wiki) for more information.

### Process Object:
``` javascript
{ dwSize: 304,
  th32ProcessID: 10316,
  cntThreads: 47,
  th32ParentProcessID: 7804,
  pcPriClassBase: 8,
  szExeFile: "csgo.exe",
  modBaseAddr: 1673789440,
  handle: 808 }
```

The `handle` and `modBaseAddr` properties are only available when opening a process and not when listing processes.

### Module Object:
``` javascript
{ modBaseAddr: 468123648,
  modBaseSize: 80302080,
  szExePath: 'c:\\program files (x86)\\steam\\steamapps\\common\\counter-strike global offensive\\csgo\\bin\\client.dll',
  szModule: 'client.dll',
  th32ProcessID: 10316,
  GlblcntUsage: 2 }
  ```

### Result Object:
``` javascript
{ returnValue: 1.23,
  exitCode: 2 }
```

The `returnValue` is the value returned from the function that was called. `exitCode` is the termination status of the thread.

### Data Type:

When using the write or read functions, the data type (dataType) parameter should reference a constant from within the library:

| Constant          | Bytes | Aliases                            | Range |
|-------------------|-------|------------------------------------|-------|
| `memoryjs.BOOL`   | 1     | `memoryjs.BOOLEAN`                 | 0 to 1 |
| `memoryjs.INT8`   | 1     | `memoryjs.BYTE`, `memoryjs.CHAR`   | -128 to 127 |
| `memoryjs.UINT8`  | 1     | `memoryjs.UBYTE`, `memoryjs.UCHAR` | 0 to 255 |
| `memoryjs.INT16`  | 2     | `memoryjs.SHORT`                   | -32,768 to 32,767 |
| `memoryjs.UINT16` | 2     | `memoryjs.USHORT`, `memoryjs.WORD` | 0 to 65,535 |
| `memoryjs.INT32`  | 4     | `memoryjs.INT`, `memoryjs.LONG`    | -2,147,483,648 to 2,147,483,647 |
| `memoryjs.UINT32` | 4     | `memoryjs.UINT`, `memoryjs.ULONG`, `memoryjs.DWORD` |	0 to 4,294,967,295 |
| `memoryjs.INT64`  | 8     | n/a                                | -9,223,372,036,854,775,808 to 9,223,372,036,854,775,807 |
| `memoryjs.UINT64` | 8     | n/a                                | 0 to 18,446,744,073,709,551,615 |
| `memoryjs.FLOAT`  | 4     | n/a                                | 3.4E +/- 38 (7 digits) |
| `memoryjs.DOUBLE` | 8     | n/a                                | 1.7E +/- 308 (15 digits) |
| `memoryjs.PTR`    | 4/8   | `memoryjs.POINTER`                 | n/a |
| `memoryjs.UPTR`   | 4/8   | `memoryjs.UPOINTER`                | n/a |
| `memoryjs.STR`    | n/a   | `memoryjs.STRING`                  | n/a |
| `memoryjs.VEC3`   | 12    | `memoryjs.VECTOR3`                 | n/a |
| `memoryjs.VEC4`   | 16    | `memoryjs.VECTOR4`                 | n/a |

**Note: pointer will be 4 bytes in a 32 bit build, and 8 bytes in a 64 bit build**

**Note: when writing 64 bit integers (`INT64`, `UINT64`, `INT64_BE`, `UINT64_BE`) you will need to supply a [BigInt](https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/BigInt). When reading a 64 bit integer, you will receive a BigInt**

These data types are to used to denote the type of data being read or written.

64 bit integer example:
```javascript
const value = memoryjs.readMemory(handle, address, memoryjs.INT64);
console.log(typeof value); // bigint
memoryjs.writeMemory(handle, address, value + 1n, memoryjs.INT64);
```

Vector3 is a data structure of three floats:
```javascript
const vector3 = { x: 0.0, y: 0.0, z: 0.0 };
memoryjs.writeMemory(address, vector3);
```

Vector4 is a data structure of four floats:
```javascript
const vector4 = { w: 0.0, x: 0.0, y: 0.0, z: 0.0 };
memoryjs.writeMemory(address, vector4);
```

### Generic Structures:

If you have a structure you want to write to memory, you can use buffers. For an example on how to do this, view the [buffers example](https://github.com/Rob--/memoryjs/blob/master/examples/buffers.js).

To write/read a structure to/from memory, you can use [structron](https://github.com/LordVonAdel/structron) to define your structures and use them to write or parse buffers.

If you want to read a `std::string` using `structron`, the library exposes a custom type that can be used to read/write strings:
```javascript
// To create the type, we need to pass the process handle, base address of the
// structure, and the target process architecture (either "32" or "64").
const stringType = memoryjs.STRUCTRON_TYPE_STRING(processObject.handle, structAddress, '64');

// Create a custom structure using the custom type, full example in /examples/buffers.js
const Struct = require('structron');
const Player = new Struct()
  .addMember(string, 'name');
```

Alternatively, you can use the [concentrate](https://github.com/deoxxa/concentrate) and [dissolve](https://github.com/deoxxa/dissolve) libraries to achieve the same thing. An old example of this is [here](https://github.com/Rob--/memoryjs/blob/aa6ed7d302fb1ac315aaa90558db43d128746912/examples/buffers.js).

### Protection Type:

Protection type is a bit flag DWORD value.

This parameter should reference a constant from the library:

`memoryjs.PAGE_NOACCESS, memoryjs.PAGE_READONLY, memoryjs.PAGE_READWRITE, memoryjs.PAGE_WRITECOPY, memoryjs.PAGE_EXECUTE, memoryjs.PAGE_EXECUTE_READ, memoryjs.PAGE_EXECUTE_READWRITE, memoryjs.PAGE_EXECUTE_WRITECOPY, memoryjs.PAGE_GUARD, memoryjs.PAGE_NOCACHE, memoryjs.PAGE_WRITECOMBINE, memoryjs.PAGE_ENCLAVE_THREAD_CONTROL, memoryjs.PAGE_TARGETS_NO_UPDATE, memoryjs.PAGE_TARGETS_INVALID, memoryjs.PAGE_ENCLAVE_UNVALIDATED`

Refer to MSDN's [Memory Protection Constants](https://docs.microsoft.com/en-gb/windows/desktop/Memory/memory-protection-constants) for more information.

### Memory Allocation Type:

Memory allocation type is a bit flag DWORD value.

This parameter should reference a constat from the library:

`memoryjs.MEM_COMMIT, memoryjs.MEM_RESERVE, memoryjs.MEM_RESET, memoryjs.MEM_RESET_UNDO`

Refer to MSDN's [VirtualAllocEx](https://docs.microsoft.com/en-us/windows/desktop/api/memoryapi/nf-memoryapi-virtualallocex) documentation for more information.

### Strings:

You can use this library to read either a "string", or "char*" and to write a string.

In both cases you want to get the address of the char array:

```c++
std::string str1 = "hello";
std::cout << "Address: 0x" << hex << (DWORD) str1.c_str() << dec << std::endl;

char* str2 = "hello";
std::cout << "Address: 0x" << hex << (DWORD) str2 << dec << std::endl;
```

From here you can simply use this address to write and read memory.

There is one caveat when reading a string in memory however, due to the fact that the library does not know
how long the string is, it will continue reading until it finds the first null-terminator. To prevent an
infinite loop, it will stop reading if it has not found a null-terminator after 1 million characters.

One way to bypass this limitation in the future would be to allow a parameter to let users set the maximum
character count.

### Signature Type:

When pattern scanning, flags need to be raised for the signature types. The signature type parameter needs to be one of the following:

`0x0` or `memoryjs.NORMAL` which denotes a normal signature.

`0x1` or `memoryjs.READ` which will read the memory at the address.

`0x2` or `memoryjs.SUBSTRACT` which will subtract the image base from the address.

To raise multiple flags, use the bitwise OR operator: `memoryjs.READ | memoryjs.SUBTRACT`.

### Function Execution:

Remote function execution works by building an array of arguments and dynamically generating shellcode that is injected into the target process and executed, for this reason crashes may occur.

To call a function in a process, the `callFunction` function can be used. The library supports passing arguments to the function and need to be in the following format:

```javascript
[{ type: T_INT, value: 4 }]
```

The library expects the arguments to be an array of objects where each object has a `type` which denotes the data type of the argument, and a `value` which is the actual value of the argument. The various supported data types can be found below.


``` javascript
memoryjs.T_VOID = 0x0,
memoryjs.T_STRING = 0x1,
memoryjs.T_CHAR = 0x2,
memoryjs.T_BOOL = 0x3,
memoryjs.T_INT = 0x4,
memoryjs.T_DOUBLE = 0x5,
memoryjs.T_FLOAT = 0x6,
```

When using `callFunction`, you also need to supply the return type of the function, which again needs to be one of the above values.

For example, given the following C++ function:

``` c++
int add(int a, int b) {
    return a + b;
}
```

You would call this function as so:

```javascript
const args = [{
    type: memoryjs.T_INT,
    value: 2,
}, {
    type: memoryjs.T_INT,
    value: 5,
}];
const returnType = T_INT;

> memoryjs.callFunction(handle, args, returnType, address);
{ returnValue: 7, exitCode: 7 }
```

See the [result object documentation](user-content-result-object) for details on what `callFunction` returns.

Notes: currently passing a `double` as an argument is not supported, but returning one is.

Much thanks to the [various contributors](https://github.com/Rob--/memoryjs/issues/6) that made this feature possible.

### Hardware Breakpoints:

Hardware breakpoints work by attaching a debugger to the process, setting a breakpoint on a certain address and declaring a trigger type (e.g. breakpoint on writing to the address) and then continuously waiting for a debug event to arise (and then consequently handling it).

This library exposes the main functions, but also includes a wrapper class to simplify the process. For a complete code example, checkout our [debugging example](https://github.com/Rob--/memoryjs/blob/master/examples/debugging.js).

When setting a breakpoint, you are required to pass a trigger type:
- `memoryjs.TRIGGER_ACCESS` - breakpoint occurs when the address is accessed
- `memoryjs.TRIGGER_WRITE` - breakpoint occurs when the address is written to

Do note that when monitoring an address containing a string, the `size` parameter of the `setHardwareBreakpoint` function should be the length of the string. When using the `Debugger` wrapper class, the wrapper will automatically determine the size of the string by attempting to read it.

To summarise:
- When using the `Debugger` class:
  - No need to pass the `size` parameter to `setHardwareBreakpoint`
  - No need to manually pick a hardware register
  - Debug events are picked up via an event listener
  - `setHardwareBreakpoint` returns the register that was used for the breakpoint

- When manually using the debugger functions:
  - The `size` parameter is the size of the variable in memory (e.g. int32 = 4 bytes). For a string, this parameter is the length of the string
  - Manually need to pick a hardware register (via `memoryjs.DR0` through `memoryhs.DR3`). Only 4 hardware registers are available (some CPUs may even has less than 4 available). This means only 4 breakpoints can be set at any given time
  - Need to manually wait for debug and handle debug events
  - `setHardwareBreakpoint` returns a boolean stating whether the operation as successful

For more reading about debugging and hardware breakpoints, checkout the following links:
- [DebugActiveProcess](https://msdn.microsoft.com/en-us/library/windows/desktop/ms679295(v=vs.85).aspx) - attatching the debugger
- [DebugSetProcessKillOnExit](https://docs.microsoft.com/en-us/windows/desktop/api/winbase/nf-winbase-debugsetprocesskillonexit) - kill the process when detatching
- [DebugActiveProcessStop](https://msdn.microsoft.com/en-us/library/windows/desktop/ms679296(v=vs.85).aspx) - detatching the debugger
- [WaitForDebugEvent](https://msdn.microsoft.com/en-us/library/windows/desktop/ms681423(v=vs.85).aspx) - waiting for the breakpoint to be triggered
- [ContinueDebugEvent](https://msdn.microsoft.com/en-us/library/windows/desktop/ms679285(v=vs.85).aspx) - handling the event

#### Using the Debugger Wrapper

The Debugger wrapper contains these functions you should use:

``` javascript
class Debugger {
  attatch(processId, killOnDetatch = false);
  detatch(processId);
  setHardwareBreakpoint(processId, address, trigger, dataType);
  removeHardwareBreakpoint(processId, register);
}
```

1. Attach the debugger
``` javascript
const hardwareDebugger = memoryjs.Debugger;
hardwareDebugger.attach(processId);
```

2. Set a hardware breakpoint
``` javascript
const address = 0xDEADBEEF;
const trigger = memoryjs.TRIGGER_ACCESS;
const dataType = memoryjs.INT;
const register = hardwareDebugger.setHardwareBreakpoint(processId, address, trigger, dataType);
```

3. Create an event listener for debug events (breakpoints)
``` javascript
// `debugEvent` event emission catches debug events from all registers
hardwareDebugger.on('debugEvent', ({ register, event }) => {
  console.log(`Hardware Register ${register} breakpoint`);
  console.log(event);
});

// You can listen to debug events from specific hardware registers
// by listening to whatever register was returned from `setHardwareBreakpoint`
hardwareDebugger.on(register, (event) => {
  console.log(event);
});
```

#### When Manually Debugging

1. Attatch the debugger
``` javascript
const hardwareDebugger = memoryjs.Debugger;
hardwareDebugger.attach(processId);
```

2. Set a hardware breakpoint (determine which register to use and the size of the data type)
``` javascript
// available registers: DR0 through DR3
const register = memoryjs.DR0;
// int = 4 bytes
const size = 4;

const address = 0xDEADBEEF;
const trigger = memoryjs.TRIGGER_ACCESS;
const dataType = memoryjs.INT;

const success = memoryjs.setHardwareBreakpoint(processId, address, register, trigger, size);
```

3. Create the await/handle debug event loop
``` javascript
const timeout = 100;

setInterval(() => {
  // `debugEvent` can be null if no event occurred
  const debugEvent = memoryjs.awaitDebugEvent(register, timeout);

  // If a breakpoint occurred, handle it
  if (debugEvent) {
    memoryjs.handleDebugEvent(debugEvent.processId, debugEvent.threadId);
  }
}, timeout);
```

Note: a loop is not required, e.g. no loop required if you want to simply wait until the first detection of the address being accessed or written to.

# Debug

### 1. Re-compile the project to be debugged

Go to the root directory of the module and run one of the following commands:
```bash
# will automatically compile based on the detected Node architecture
npm run debug

# compile to target 32 bit processes
npm run debug32

# compile to target 64 bit processes
npm run debug64
```

### 2. Change the `index.js` file to require the debug module

Go to the root directory and change the line in `index.js` from:
```javascript
const memoryjs = require('./build/Release/memoryjs');
```

To the following:
```javascript
const memoryjs = require('./build/Debug/memoryjs');
```

### 3. Open the project in Visual Studio

Open the `binding.sln` solution in Visual Studio, found in the `build` folder in the project's root directory.

### 4. Setup Visual Studio debug configuration

  1. In the toolbar, click "Project" then "Properties"
  2. Under "Configuration Properties", click "Debugging"
  3. Set the "Command" property to the location of your `node.exe` file (e.g. `C:\nodejs\node.exe`)
  4. Set the "Command Arguments" property to the location of your script file (e.g. `C:\project\test.js`)

### 5. Set breakpoints

Explore the project files in Visual Studio (by expanding `..` and then `lib` in the Solution Explorer). Header files can be viewed by holding `Alt` and clicking on the header file names at the top of the source code files.

Breakpoints are set by clicking to the left of the line number.

### 6. Run the debugger

Start debugging by either pressing `F5`, by clicking "Debug" in the toolbar and then "Start Debugging", or by clicking "Local Windows Debugger".

The script you've set as the command argument in step 4 will be run, and Visual Studio will pause on the breakpoints set and allow you to step through the code line by line and inspect variables.
