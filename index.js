var memoryjs = require('./build/Release/memoryjs');

var processName = "chrome.exe";

if(memoryjs.openProcess(processName)){
  console.log("Successfully opened", processName);
} else {
  console.log("Unable to open", processName);
}
