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

# Usage

To simply open a process:
``` javascript
var memoryjs = require('./build/Release/memoryjs');

if(memoryjs.openProcess("chrome.exe")){
  console.log("Successfully opened", processName);
} else {
  console.log("Unable to open", processName);
}
```
