# memoryjs
Node add-on for memory reading and writing! (finally!)

# Features

- open a process
- close the process (handle)
- list all open processes
- list all modules associated with a process
- find a certain module associated with a process
- read memory
- write to memory

TODO:
- pattern scanning

# Install

This is a Node add-on and therefore requires [node-gyp](https://github.com/nodejs/node-gyp) to use.
This is currently in development, therefore to test the add-on you must:

- clone/download the repository
- open `compile.bat` or run `node-gyp rebuild` (or `node-gyp configure` followed by `node-gyp build`)
- then just run `node index.js` to run the exemplar

# Usage

For a complete example, view `index.js`.

Initialise:
``` javascript
var memoryjs = require('./build/Release/memoryjs');
var processName = "chrome.exe";
```

### Processes

Open a process (sync):
``` javascript
var process = memoryjs.openProcess(processName);
```

Open a process (async):
``` javascript
memoryjs.openProcess(processName, function(process){

});
```

Get all processes (sync):
``` javascript
var processes = memoryjs.getProcesses();
```

Get all processes (async):
``` javascript
memoryjs.getProcesses(function(processes){

});
```

See the `Documentation` section of this README to see what a process object looks like.

### Modules

Find a module (sync):
``` javascript
var module = memoryjs.findModule(moduleName, processId);
```

Find a module (async):
``` javascript
memoryjs.findModule(moduleName, processId, function(module){

});
```

Get all modules (sync):
``` javascript
var modules = memoryjs.getModules(processId);
```

Get all modules (async):
``` javascript
memoryjs.getModules(processId, function(modules){

});
```

See the [Documentation](#user-content-documentation) section of this README to see what a module object looks like.

### Memory

Read from memory (sync):
``` javascript
memoryjs.readMemory(address, dataType);
```

Read from memory (async):
``` javascript
memoryjs.readMemory(address, dataType, function(result){

});
```

Write to memory:
``` javascript
memoryjs.writeMemory(address, value, dataType);
```

See the [Documentation](#user-content-documentation) section of this README to see what values `dataType` can be.

# Documentation

Process object:
``` javascript
{  cntThreads: 47,
   szExeFile: "csgo.exe",
   th32ProcessID: 10316,
   th32ParentProcessID: 7804,
   pcPriClassBase: 8 }
```

Module object:
``` javascript
{ modBaseAddr: 468123648,
  modBaseSize: 80302080,
  szExePath: 'c:\\program files (x86)\\steam\\steamapps\\common\\counter-strike global offensive\\csgo\\bin\\client.dll',
  szModule: 'client.dll',
  th32ProcessID: 10316 }
  ```

  When using the write or read functions, the data type parameter must be a string and be one of the following:

  `"int", "dword", "long", "float", "double", "bool", "boolean", "str", "string"`

  This is simply used to denote the type of data being read or written.

#### openProcess(processName[, callback])
---

opens a process to be able to read from and write to it

- **processName** *(string)* - the name of the process to open
- **callback** *(function)* - has one parameter (process)

**returns** *process object* either directly or via the callback

#### closeProcess()
---

closes the handle on the opened process

#### getProcesses([callback])
---

collects information about all the running processes

- **callback** *(function)* - has one parameter (array of processes)

**returns** an array of *process object* for all the running processes

#### findModule(moduleName, processId[, callback])
---

finds a module associated with a given process

- **moduleName** *(string)* - the name of the module to find
- **processId** *(int)* - the id of the process in which to find the module
- **callback** *(function)* - has one parameter (module)

**returns** *module object* either directly or via the callback

#### getModules(processId[, callback])
---

gets all modules associated with a given process

- **processId** *(int)* - the id of the process in which to find the module
- **callback** *(function)* - has one parameter (array of modules)

**returns** an array of *module object* for all the modules found

#### readMemory(address, dataType[, callback])
---

reads the memory at a given address

- **address** *(int)* - the address in memory to read from
- **dataType** *(string)* - the data type to read into (definitions can be found at the top of this section)
- **callback** *(function)* - has one parameter (the value)

**returns** the value that has been read from memory

#### writeMemory(address, value, dataType)
---

writes to an address in memory

- **address** *(int)* - the address in memory to write to
- **value** *(any data type)* - the data type of value must be either `number`, `string` or `boolean` and is the value that will be written to the address in memory
- **dataType** *(string)* the data type of the value (definitions can be found at the top of this section)
