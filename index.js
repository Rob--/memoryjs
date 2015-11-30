var memoryjs = require('./build/Release/memoryjs');
var processName = "chrome.exe";

// open a process (sync)
var process = memoryjs.openProcess(processName);

// open a process (async)
memoryjs.openProcess(processName, function (process) {
    console.log(JSON.stringify(process, null, 3));
    if (process.szExeFile) {
        console.log("Successfully opened handle on", processName);

        memoryjs.closeProcess(process.handle);
        console.log("Closed handle on", processName)
    } else {
        console.log("Unable to open handle on", processName);
    }
});

// get all processes (sync)
var processes = memoryjs.getProcesses();
console.log("\nGetting all processes (sync)\n---\n");
for (var i = 0; i < processes.length; i++) {
    console.log(processes[i].szExeFile);
}

// get all processes (async)
console.log("\nGetting all processes (async)\n---\n");
memoryjs.getProcesses(function(processes){
    for (var i = 0; i < processes.length; i++) {
        console.log(processes[i].szExeFile);
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

// get all modules (sync)
console.log("\nGetting all modules (sync)\n---\n");
var modules = memoryjs.getModules(process.th32ProcessID);
for (var i = 0; i < modules.length; i++) {
    console.log(modules[i].szExePath);
}

// get all modules (async)
console.log("\nGetting all modules (async)\n---\n");
memoryjs.getModules(process.th32ProcessID, function (modules) {
    for (var i = 0; i < modules.length; i++) {
        console.log(modules[i].szModule);
    }
});

// find a module associated with a process (sync)
console.log("\nFinding module \"ntdll.dll\" (sync)\n---\n");
console.log(memoryjs.findModule("ntdll.dll", process.th32ProcessID));

// find a module associated with a process (async)
console.log("\nFinding module \"ntdll.dll\" (async)\n---\n");
memoryjs.findModule("ntdll.dll", process.th32ProcessID, function (module) {
    console.log(module.szModule);
});

/* module =
{ dwSize: 568,
  GlblcntUsage: 65535,
  modBaseAddr: -1870135296,
  modBaseSize: 1839104,
  ProccntUsage: 65535,
  szExePath: 'C:\\WINDOWS\\SYSTEM32\\ntdll.dll',
  szModule: 'ntdll.dll',
  th32ModuleID: 5844 } */