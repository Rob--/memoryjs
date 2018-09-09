const memoryjs = require('./index');
const processName = 'csgo.exe';
let clientModule;
const offset = 0x00A9D44C;

// open a process (sync)
const processObject = memoryjs.openProcess(processName);

// open a process (async)
memoryjs.openProcess(processName, (error, processObject) => {
  console.log(JSON.stringify(processObject, null, 3));

  if (process.szExeFile) {
    console.log('Successfully opened handle on', processName);

    memoryjs.closeProcess(processObject.handle);
    console.log('Closed handle on', processName);
  } else {
    console.log('Unable to open handle on', processName);
  }
});

// get all processes (sync)
const processes = memoryjs.getProcesses();
console.log('\nGetting all processes (sync)\n---\n');
processes.forEach(({ szExeFile }) => console.log(szExeFile));

// get all processes (async)
console.log('\nGetting all processes (async)\n---\n');
memoryjs.getProcesses((error, processes) => {
  processes.forEach(({ szExeFile }) => console.log(szExeFile));
});

/* process =
{  cntThreads: 47,
   szExeFile: "csgo.exe",
   th32ProcessID: 10316,
   th32ParentProcessID: 7804,
   pcPriClassBase: 8 } */

// get all modules (sync)
console.log('\nGetting all modules (sync)\n---\n');
const modules = memoryjs.getModules(processObject.th32ProcessID);
modules.forEach(({ szExeFile }) => console.log(szExeFile));

// get all modules (async)
console.log('\nGetting all modules (async)\n---\n');
memoryjs.getModules(processObject.th32ProcessID, (error, modules) => {
  modules.forEach(({ szExeFile }) => console.log(szExeFile));
});

// find a module associated with a process (sync)
console.log('\nFinding module "client.dll" (sync)\n---\n');
console.log(memoryjs.findModule('client.dll', processObject.th32ProcessID));

// find a module associated with a process (async)
console.log('\nFinding module "client.dll" (async)\n---\n');
memoryjs.findModule('client.dll', processObject.th32ProcessID, (error, module) => {
  console.log(module.szModule);
  clientModule = module;
});

/* module =
{ modBaseAddr: 468123648,
  modBaseSize: 80302080,
  szExePath: 'c:\\program files (x86)\\steam\\steamapps\\common\\counter-strike global offensive\\csgo\\bin\\client.dll',
  szModule: 'client.dll',
  th32ProcessID: 10316 } */

const address = clientModule.modBaseAddr + offset;

// read memory (sync)
console.log(`value of 0x${address.toString(16)}: ${memoryjs.readMemory(processObject.handle, address, memoryjs.INT)}`);

// read memory (async)
memoryjs.readMemory(processObject.handle, address, memoryjs.INT, (error, result) => {
  console.log(`value of 0x${address.toString(16)}: ${result}`);
});

// write memory
memoryjs.writeMemory(processObject.handle, address, 1, memoryjs.INT);

// pattern reading
const signature = 'A3 ? ? ? ? C7 05 ? ? ? ? ? ? ? ? E8 ? ? ? ? 59 C3 6A';
const signatureTypes = memoryjs.READ | memoryjs.SUBTRACT;
const patternOffset = 0x1;
const addressOffset = 0x10;
const dwLocalPlayer = memoryjs.findPattern(processObject.handle, clientModule.szModule, signature, signatureTypes, patternOffset, addressOffset);
console.log(`value of dwLocalPlayer: 0x${dwLocalPlayer.toString(16)}`);
