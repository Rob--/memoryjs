# memoryjs &middot; [![GitHub license](https://img.shields.io/badge/license-MIT-blue.svg)](https://github.com/Rob--/memoryjs/blob/master/LICENSE.md) [![npm version](https://img.shields.io/npm/v/memoryjs.svg?style=flat)](https://www.npmjs.com/package/memoryjs)

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
- Pattern scanning
- Execute a function within a process

Functions that this library directly exposes from the WinAPI:
- [ReadProcessMemory](https://docs.microsoft.com/en-us/windows/desktop/api/memoryapi/nf-memoryapi-readprocessmemory)
- [WriteProcessMemory](https://docs.microsoft.com/en-us/windows/desktop/api/memoryapi/nf-memoryapi-writeprocessmemory)
- [VirtualProtectEx](https://docs.microsoft.com/en-us/windows/desktop/api/memoryapi/nf-memoryapi-virtualprotectex)
- [VirtualAllocEx](https://docs.microsoft.com/en-us/windows/desktop/api/memoryapi/nf-memoryapi-virtualallocex)

TODO:
- WriteFile support (for driver interactions)
- DLL injections

# Install

This is a Node add-on (last tested to be working on `v8.11.3`) and therefore requires [node-gyp](https://github.com/nodejs/node-gyp) to use.

You may also need to [follow these steps](https://github.com/nodejs/node-gyp#user-content-installation).

`npm install memoryjs`

When using memoryjs, the target process should match the platform architecture of the Node version running.
For example if you want to target a 64 bit process, you should try and use a 64 bit version of Node.

You also need to recompile the library and target the platform you want. Head to the memoryjs node module directory, open up a terminal and to run the compile scripts, type:

`npm run build32` if you want to target 32 bit processes

`npm run build64` if you want to target 64 bit processes

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
const offset = memoryjs.findPattern(handle, moduleName, signature, signatureType, patternOffset, addressOffset);
```

Pattern scanning (async):
``` javascript
memoryjs.findPattern(handle, moduleName, signature, signatureType, patternOffset, addressOffset, (error, offset) => {

})
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

Click [here](#user-content-result-object) to see what a result object looks like.
Clicklick [here](#user-content-function-execution-1) for details about how to format the arguments and the return type.

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
  th32ProcessID: 10316 }
  ```

### Result Object:
``` javascript
{ returnValue: 1.23,
  exitCode: 2 }
```

The `returnValue` is the value returned from the function that was called. `exitCode` is the termination status of the thread.

### Data Type:

When using the write or read functions, the data type (dataType) parameter can either be a string and be one of the following:

`"byte", "int", "int32", "uint32", "int64", "uint64", "dword", "short", "long", "float", "double", "bool", "boolean", "ptr", "pointer", "str", "string", "vec3", "vector3", "vec4", "vector4"`

or can reference constants from within the library:

`memoryjs.BYTE, memoryjs.INT, memoryjs.INT32, memoryjs.UINT32, memoryjs.INT64, memoryjs.UINT64, memoryjs.DWORD, memoryjs.SHORT, memoryjs.LONG, memoryjs.FLOAT, memoryjs.DOUBLE, memoryjs.BOOL, memoryjs.BOOLEAN, memoryjs.PTR, memoryjs.POINTER, memoryjs.STR, memoryjs.STRING, memoryjs.VEC3, memoryjs.VECTOR3, memoryjs.VEC4, memoryjs.VECTOR4`

This is simply used to denote the type of data being read or written.

Vector3 is a data structure of three floats:

``` javascript
const vector3 = { x: 0.0, y: 0.0, z: 0.0 };
memoryjs.writeMemory(address, vector3);
```

Vector4 is a data structure of four floats:

``` javascript
const vector4 = { w: 0.0, x: 0.0, y: 0.0, z: 0.0 };
memoryjs.writeMemory(address, vector4);
```

### Generic Structures:

If you have a structure you want to write to memory, you can use buffers. For an example on how to do this, view the [buffers example](https://github.com/Rob--/memoryjs/blob/master/examples/buffers.js).

To write a structure to memory, you can use the [concentrate](https://github.com/deoxxa/concentrate) library to describe the structure as a buffer
and then write the buffer to memory using the `writeBuffer` function.

To read a structure from memory, you will need to read a buffer from memory using the `readBuffer` function, and then you can use the [dissolve](https://github.com/deoxxa/dissolve) library to parse the buffer into a structure.

In either case you don't need to use the two libraries mentioned above, they just make it easy to turn your structure into a buffer, and your buffer into a structure.

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
