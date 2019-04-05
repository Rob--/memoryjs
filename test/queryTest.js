const memoryjs = require('../index');
const processName = 'chrome.exe';

const processObject = memoryjs.openProcess(processName);

const regions = memoryjs.getRegions(processObject.handle);

console.log(JSON.stringify(regions, null, 3));

memoryjs.closeProcess(processObject.handle);
