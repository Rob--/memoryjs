const memoryjs = require('../index');
const processName = 'chrome.exe';

const processObject = memoryjs.openProcess(processName);

const regions = memoryjs.getRegions(processObject.handle);

// console.log(`Allocated address: 0x${address.toString(16).toUpperCase()}`);

console.log(JSON.stringify(regions, null, 3));

memoryjs.closeProcess(processObject.handle);
