var memoryjs = require('./build/Release/memoryjs');
var processName = "chrome.exe";

// sync
var process = memoryjs.openProcess(processName);

// async
memoryjs.openProcess(processName, function(process){
    if (process.szExeFile) {
        console.log("Successfully opened handle on", processName);

        memoryjs.closeProcess(process.handle);
        console.log("Closed handle on", processName)
    } else {
        console.log("Unable to open handle on", processName);
    }
});

/* process =
{ cntThreads: 14,
  cntUsage: 0,
  dwFlags: 0,
  dwSize: 304,
  szExeFile: 'chrome.exe',
  th32ProcessID: 10044,
  th32ParentProcessID: 5868 } */