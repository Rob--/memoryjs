# memoryjs
Node add-on for memory reading and writing! (finally!)

# Features

- open a process

TODO:
- list all open processes
- list modules associated with a process
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

Open a process (sync):
``` javascript
var process = memoryjs.openProcess(processName);
```

Open a process (async):
``` javascript
memoryjs.openProcess(processName, function(process){

});
```

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

Get all processes (sync):
``` javascript
var processes = memoryjs.getProcesses();
```

Get all processes (async):
``` javascript
memoryjs.getProcesses(function(processes){

});
```
