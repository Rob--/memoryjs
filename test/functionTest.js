const memoryjs = require('../index');
const processName = 'Target Process.exe';

const processObject = memoryjs.openProcess(processName);

const x = memoryjs.callFunction(processObject.handle, [{ type: memoryjs.T_FLOAT, value: 12.34 }], memoryjs.T_FLOAT, 0xad1537);
console.log(x);
memoryjs.closeProcess(processObject.handle);