# memoryjs
Node add-on for memory reading and writing! (finally!)

# Features

- open a process
- close the process (handle)
- list all open processes
- list all modules associated with a process
- find a certain module associated with a process
- read memory

TODO:
- write to memory
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

A process object looks like this:
``` javascript
// process =
{  cntThreads: 47,
   szExeFile: "csgo.exe",
   th32ProcessID: 10316,
   th32ParentProcessID: 7804,
   pcPriClassBase: 8 }
```

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

A module object looks like this:
``` javascript
// module =
{ modBaseAddr: 468123648,
  modBaseSize: 80302080,
  szExePath: 'c:\\program files (x86)\\steam\\steamapps\\common\\counter-strike global offensive\\csgo\\bin\\client.dll',
  szModule: 'client.dll',
  th32ProcessID: 10316 }
  ```

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