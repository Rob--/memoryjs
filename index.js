var memoryjs = require('./build/Release/memoryjs');
var processName = "chrome.exe";

memoryjs.openProcess(processName, function(process){
  if(process){
    console.log("Successfully opened handle on", processName);

    memoryjs.closeProcess(process);
    console.log("Closed handle on", processName)
  } else {
    console.log("Unable to open handle on", processName);
  }
});
