var memoryjs = require('./build/Release/memoryjs');

module.exports = {

  INT: 'int',
  DWORD: 'dword',
  LONG: 'long',
  FLOAT: 'float',
  DOUBLE: 'double',
  BOOL: 'bool',
  BOOLEAN: 'boolean',
  STR: 'str',
  STRING: 'string',

  openProcess: function (processName, callback) {
    if(arguments.length == 1) return memoryjs.openProcess(processName);
    else memoryjs.openProcess(processName, callback);
  },

  getProcesses: function(callback){
    if(arguments.length == 0) return memoryjs.getProcesses();
    else memoryjs.getProcesses(callback);
  },

  findModule: function(moduleName, processId, callback){
    if(arguments.length == 2) return memoryjs.findModule(moduleName, processId);
    else memoryjs.findModule(moduleName, processId, callback);
  },

  getModules: function(processId, callback){
    if(arguments.length == 1) return memoryjs.getModules(processId);
    else memoryjs.getModules(processId, callback);
  },

  readMemory: function(address, dataType, callback){
    if(arguments.length == 2) return memoryjs.readMemory(address, dataType);
    else memoryjs.readMemory(address, dataType, callback);
  },

  writeMemory: function(address, value, dataType, callback){
    if(arguments.length == 3) return memoryjs.writeMemory(address, value, dataType);
    else memoryjs.writeMemory(address, value, dataType, callback);
  }
};
