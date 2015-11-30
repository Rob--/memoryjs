# memoryjs
Node add-on for memory reading and writing! (finally!)

# Features

- open a process
- close the process (handle)
- list all open processes
- list all modules associated with a process
- find a certain module associated with a process

TODO:
- read memory
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

### Process

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
{ cntThreads: 14,
  cntUsage: 0,
  dwFlags: 0,
  dwSize: 304,
  szExeFile: 'chrome.exe',
  th32ProcessID: 10044,
  th32ParentProcessID: 5868 }
```

### Module

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
{ dwSize: 568,
  GlblcntUsage: 65535,
  modBaseAddr: -1870135296,
  modBaseSize: 1839104,
  ProccntUsage: 65535,
  szExePath: 'C:\\WINDOWS\\SYSTEM32\\ntdll.dll',
  szModule: 'ntdll.dll',
  th32ModuleID: 5844 }
  ```
