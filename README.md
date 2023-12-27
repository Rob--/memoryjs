<p align="center">
  <img width="600" src="assets/logo.png">
  <br>
  <code>memoryjs</code> is a an NPM package to read and write process memory!
</p>

<p align="center">
  <img src="https://img.shields.io/github/license/Rob--/memoryjs" alt="GitHub License">
  <img src="https://img.shields.io/npm/v/memoryjs.svg?style=flat" alt="NPM Version">
  <img src="https://img.shields.io/npm/dy/memoryjs" alt="NPM Downloads">
</p>

---

<p align="center">
  <a href="#user-content-features">Features</a> •
  <a href="#user-content-getting-started">Getting Started</a> •
  <a href="#user-content-usage">Usage</a> •
  <a href="#user-content-documentation">Documentation</a> •
  <a href="#user-content-debug">Debug</a>
</p>


# Features

- List all open processes
- List all modules associated with a process
- Close process/file handles
- Find a specific module within a process
- Read and write process memory (w/big-endian support)
- Read and write buffers (arbitrary structs)
- Change memory protection
- Reserve/allocate, commit or change regions of memory
- Fetch a list of memory regions within a process
- Pattern scanning
- Execute a function within a process
- Hardware breakpoints (find out what accesses/writes to this address, etc)
- Inject & unload DLLs
- Read memory mapped files

TODO:
- WriteFile support (for driver interactions)
- Async/await support

# Getting Started

## Install

This is a Node add-on (last tested to be working on `v14.15.0`) and therefore requires [node-gyp](https://github.com/nodejs/node-gyp) to use.

You may also need to [follow these steps](https://github.com/nodejs/node-gyp#user-content-installation) to install and setup `node-gyp`.

```bash
npm install memoryjs
```

When using memoryjs, the target process should match the platform architecture of the Node version running.
For example if you want to target a 64 bit process, you should try and use a 64 bit version of Node.

You also need to recompile the library and target the platform you want. Head to the memoryjs node module directory, open up a terminal and run one of the following compile scripts:

```bash
# will automatically compile based on the detected Node architecture
npm run build

# compile to target 32 bit processes
npm run build32

# compile to target 64 bit processes
npm run build64
```

## Node Webkit / Electron

If you are planning to use this module with Node Webkit or Electron, take a look at [Liam Mitchell](https://github.com/LiamKarlMitchell)'s build notes [here](https://github.com/Rob--/memoryjs/issues/23).

# Usage

## Initialise
``` javascript
const memoryjs = require('memoryjs');
const processName = "csgo.exe";
```

## Processes
- Open a process
- Get all processes
- Close a process (release handle)

```javascript
// sync: open a process
const processObject = memoryjs.openProcess(processName);

// async: open a process
memoryjs.openProcess(processName, (error, processObject) => {});


// sync: get all processes
const processes = memoryjs.getProcesses();

// async: get all processes
memoryjs.getProcesses((error, processes) => {});


// close a process (release handle)
memoryjs.closeProcess(handle);
```

See the [Documentation](#user-content-process-object) section of this README to see what a process object looks like.

## Modules 
- Find a module
- Get all modules

``` javascript
// sync: find a module
const moduleObject = memoryjs.findModule(moduleName, processId);

// async: find a module
memoryjs.findModule(moduleName, processId, (error, moduleObject) => {});


// sync: get all modules
const modules = memoryjs.getModules(processId);

// async: get all modules
memoryjs.getModules(processId, (error, modules) => {});
```

See the [Documentation](#user-content-module-object) section of this README to see what a module object looks like.

## Memory
- Read data type from memory
- Read buffer from memory
- Write data type to memory
- Write buffer to memory
- Fetch memory regions

``` javascript
// sync: read data type from memory
const value = memoryjs.readMemory(handle, address, dataType);

// async: read data type from memory
memoryjs.readMemory(handle, address, dataType, (error, value) => {});


// sync: read buffer from memory
const buffer = memoryjs.readBuffer(handle, address, size);

// async: read buffer from memory
memoryjs.readBuffer(handle, address, size, (error, buffer) => {});


// sync: write data type to memory
memoryjs.writeMemory(handle, address, value, dataType);


// sync: write buffer to memory
memoryjs.writeBuffer(handle, address, buffer);


// sync: fetch memory regions
const regions = memoryjs.getRegions(handle);

// async: fetch memory regions
memoryjs.getRegions(handle, (regions) => {});
```

See the [Documentation](#user-content-documentation) section of this README to see what values `dataType` can be.

## Memory Mapped Files
- Open a named file mapping object
- Map a view of a file into a specified process
- Close handle to the file mapping object

```javascript
// sync: open a named file mapping object
const fileHandle = memoryjs.openFileMapping(fileName);


// sync: map entire file into a specified process
const baseAddress = memoryjs.mapViewOfFile(processHandle, fileName);


// sync: map portion of a file into a specified process
const baseAddress = memoryjs.mapViewOfFile(processHandle, fileName, offset, viewSize, pageProtection);


// sync: close handle to a file mapping object
const success = memoryjs.closeProcess(fileHandle);
```

See the [Documentation](#user-content-documentation) section of this README to see details on the parameters and return values for these functions.

## Protection
- Change/set the protection on a region of memory
  
```javascript
// sync: change/set the protection on a region of memory
const oldProtection = memoryjs.virtualProtectEx(handle, address, size, protection);
```

See the [Documentation](#user-content-protection-type) section of this README to see what values `protection` can be.

## Pattern Scanning
- Pattern scan all modules and memory regions
- Pattern scan a given module
- Pattern scan a memory region or module at the given base address

```javascript
// sync: pattern scan all modules and memory regions
const address = memoryjs.findPattern(handle, pattern, flags, patternOffset);

// async: pattern scan all modules and memory regions
memoryjs.findPattern(handle, pattern, flags, patternOffset, (error, address) => {});


// sync: pattern scan a given module
const address = memoryjs.findPattern(handle, moduleName, pattern, flags, patternOffset);

// async: pattern scan a given module
memoryjs.findPattern(handle, moduleName, pattern, flags, patternOffset, (error, address) => {});


// sync: pattern scan a memory region or module at the given base address
const address = memoryjs.findPattern(handle, baseAddress, pattern, flags, patternOffset);

// async: pattern scan a memory region or module at the given base address
memoryjs.findPattern(handle, baseAddress, pattern, flags, patternOffset, (error, address) => {});
```

## Function Execution
- Execute a function in a remote process

``` javascript
// sync: execute a function in a remote process
const result = memoryjs.callFunction(handle, args, returnType, address);

// async: execute a function in a remote process
memoryjs.callFunction(handle, args, returnType, address, (error, result) => {});
```

Click [here](#user-content-result-object) to see what a result object looks like.

Click [here](#user-content-function-execution-1) for details about how to format the arguments and the return type.

## DLL Injection
- Inject a DLL
- Unload a DLL by module base address
- Unload a DLL by module name

```javascript
// sync: inject a DLL
const success = memoryjs.injectDll(handle, dllPath);

// async: inject a DLL
memoryjs.injectDll(handle, dllPath, (error, success) => {});


// sync: unload a DLL by module base address
const success = memoryjs.unloadDll(handle, moduleBaseAddress);

// async: unload a DLL by module base address
memoryjs.unloadDll(handle, moduleBaseAddress, (error, success) => {});


// sync: unload a DLL by module name
const success = memoryjs.unloadDll(handle, moduleName);

// async: unload a DLL by module name
memoryjs.unloadDll(handle, moduleName, (error, success) => {});
```

## Hardware Breakpoints
- Attach debugger
- Detach debugger
- Wait for debug event
- Handle debug event
- Set hardware breakpoint
- Remove hardware breakpoint

``` javascript
// sync: attach debugger
const success = memoryjs.attachDebugger(processId, exitOnDetach);

// sync: detach debugger
const success = memoryjs.detachDebugger(processId);

// sync: wait for debug event
const success = memoryjs.awaitDebugEvent(hardwareRegister, millisTimeout);

// sync: handle debug event
const success = memoryjs.handleDebugEvent(processId, threadId);

// sync: set hardware breakpoint
const success = memoryjs.setHardwareBreakpoint(processId, address, hardwareRegister, trigger, length);

// sync: remove hardware breakpoint
const success = memoryjs.removeHardwareBreakpoint(processId, hardwareRegister);
```

# Documentation

Note: this documentation is currently being updated, refer to the [Wiki](https://github.com/Rob--/memoryjs/wiki) for more information.

## Process Object
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

## Module Object
``` javascript
{ modBaseAddr: 468123648,
  modBaseSize: 80302080,
  szExePath: 'c:\\program files (x86)\\steam\\steamapps\\common\\counter-strike global offensive\\csgo\\bin\\client.dll',
  szModule: 'client.dll',
  th32ProcessID: 10316,
  GlblcntUsage: 2 }
  ```

## Result Object
``` javascript
{ returnValue: 1.23,
  exitCode: 2 }
```

This object is returned when a function is executed in a remote process:
- `returnValue` is the value returned from the function that was called
- `exitCode` is the termination status of the thread

## Data Types

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


Notes:
- all functions that accept an address also accept the address as a BigInt
- pointer will be 4 bytes in a 32 bit build, and 8 bytes in a 64 bit build.
- to read in big-endian mode, append `_BE` to the data type. For example: `memoryjs.DOUBLE_BE`.
- when writing 64 bit integers (`INT64`, `UINT64`, `INT64_BE`, `UINT64_BE`) you will need to supply a [BigInt](https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/BigInt). When reading a 64 bit integer, you will receive a BigInt.

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
memoryjs.writeMemory(handle, address, vector3, memoryjs.VEC3);
```

Vector4 is a data structure of four floats:
```javascript
const vector4 = { w: 0.0, x: 0.0, y: 0.0, z: 0.0 };
memoryjs.writeMemory(handle, address, vector4, memoryjs.VEC4);
```

## Generic Structures

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

## Protection Type

Protection type is a bit flag DWORD value.

This parameter should reference a constant from the library:

`memoryjs.PAGE_NOACCESS, memoryjs.PAGE_READONLY, memoryjs.PAGE_READWRITE, memoryjs.PAGE_WRITECOPY, memoryjs.PAGE_EXECUTE, memoryjs.PAGE_EXECUTE_READ, memoryjs.PAGE_EXECUTE_READWRITE, memoryjs.PAGE_EXECUTE_WRITECOPY, memoryjs.PAGE_GUARD, memoryjs.PAGE_NOCACHE, memoryjs.PAGE_WRITECOMBINE, memoryjs.PAGE_ENCLAVE_THREAD_CONTROL, memoryjs.PAGE_TARGETS_NO_UPDATE, memoryjs.PAGE_TARGETS_INVALID, memoryjs.PAGE_ENCLAVE_UNVALIDATED`

Refer to MSDN's [Memory Protection Constants](https://docs.microsoft.com/en-gb/windows/desktop/Memory/memory-protection-constants) for more information.

## Memory Allocation Type

Memory allocation type is a bit flag DWORD value.

This parameter should reference a constat from the library:

`memoryjs.MEM_COMMIT, memoryjs.MEM_RESERVE, memoryjs.MEM_RESET, memoryjs.MEM_RESET_UNDO`

Refer to MSDN's [VirtualAllocEx](https://docs.microsoft.com/en-us/windows/desktop/api/memoryapi/nf-memoryapi-virtualallocex) documentation for more information.

## Strings

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

### Signature Type

When pattern scanning, flags need to be raised for the signature types. The signature type parameter needs to be one of the following:

`0x0` or `memoryjs.NORMAL` which denotes a normal signature.

`0x1` or `memoryjs.READ` which will read the memory at the address.

`0x2` or `memoryjs.SUBSTRACT` which will subtract the image base from the address.

To raise multiple flags, use the bitwise OR operator: `memoryjs.READ | memoryjs.SUBTRACT`.

## Memory Mapped Files

The library exposes functions to map obtain a handle to and read a memory mapped file.

**openFileMapping(fileName)**
- *fileName*: name of the file mapping object to be opened
- returns: handle to the file mapping object

Refer to [MSDN's OpenFileMappingA](https://learn.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-openfilemappinga) documentation for more information.

**mapViewOfFile(processHandle, fileName)**
- *processHandle*: the target process to map the file to
- *fileHandle*: handle of the file mapping object, obtained by `memoryjs.openFileMapping`
- Description: maps the entire file to target process' memory. Page protection defaults to `constants.PAGE_READONLY`.
- Returns: the base address of the mapped file

**mapViewOfFile(processHandle, fileName, offset, viewSize, pageProtection)**
- *processHandle*: the target process to map the file to
- *fileHandle*: handle of the file mapping object, obtained by `memoryjs.openFileMapping`
- *offset* (`number` or `bigint`): the offset from the beginning of the file (has to be multiple of 64KB)
- *viewSize* (`number` or `bigint`): the number of bytes to map (if `0`, the entire file will be read, regardless of offset)
- *pageProtection*: desired page protection
- Description: maps a view of the file to the target process' memory
- Returns: the base address of the mapped file

Refer to [MSDN's MapViewOfFile2](https://learn.microsoft.com/en-us/windows/win32/api/memoryapi/nf-memoryapi-mapviewoffile2) documentation for more information.

See [Protection Type](#user-content-protection-type) for page protection types.

### Example
We have a process that creates a file mapping:
```c++
HANDLE fileHandle = CreateFileA("C:\\foo.txt", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
HANDLE fileMappingHandle = CreateFileMappingA(fileHandle, NULL, PAGE_READONLY, 0, 0, "MappedFooFile");
```

We can map the file to a specified target process and read the file with `memoryjs`:
```javascript
const processObject = memoryjs.openProcess("example.exe");
const fileHandle = memoryjs.openFileMapping("MappedFooFile");

// read entire file
const baseAddress = memoryjs.mapViewOfFile(processObject.handle, fileHandle.handle);
const data = memoryjs.readMemory(processObject.handle, baseAddress, memoryjs.STR);

// read 10 bytes after 64KB
const baseAddress = memoryjs.mapViewOfFile(processObject.handle, fileHandle.handle, 65536, 10, constants.PAGE_READONLY);
const buffer = memoryjs.readBuffer(processObject.handle, baseAddress, 10);
const data = buffer.toString();

const success = memoryjs.closeProcess(fileHandle);
```

If you want to read a memory mapped file without having a target process to map the file to, you can map it to the current Node process with global variable `process.pid`:
```javascript
const processObject = memoryjs.openProcess(process.pid);
```

## Function Execution

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

## Hardware Breakpoints

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
- [DebugActiveProcess](https://msdn.microsoft.com/en-us/library/windows/desktop/ms679295(v=vs.85).aspx) - attaching the debugger
- [DebugSetProcessKillOnExit](https://docs.microsoft.com/en-us/windows/desktop/api/winbase/nf-winbase-debugsetprocesskillonexit) - kill the process when detaching
- [DebugActiveProcessStop](https://msdn.microsoft.com/en-us/library/windows/desktop/ms679296(v=vs.85).aspx) - detaching the debugger
- [WaitForDebugEvent](https://msdn.microsoft.com/en-us/library/windows/desktop/ms681423(v=vs.85).aspx) - waiting for the breakpoint to be triggered
- [ContinueDebugEvent](https://msdn.microsoft.com/en-us/library/windows/desktop/ms679285(v=vs.85).aspx) - handling the event

### Using the Debugger Wrapper:

The Debugger wrapper contains these functions you should use:

``` javascript
class Debugger {
  attach(processId, killOnDetach = false);
  detach(processId);
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

### When Manually Debugging:

1. Attach the debugger
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
