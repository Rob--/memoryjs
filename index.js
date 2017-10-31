var memoryjs = require('./build/Release/memoryjs');

module.exports = {

  /* data type constants */
  INT: 'int',
  DWORD: 'dword',
  LONG: 'long',
  FLOAT: 'float',
  DOUBLE: 'double',
  BOOL: 'bool',
  BOOLEAN: 'boolean',
  STR: 'str',
  STRING: 'string',
  VEC2: 'vec2',
  VEC3: 'vec3',
  VEC4: 'vec4',
  VECTOR2: 'vector2',
  VECTOR3: 'vector3',
  VECTOR4: 'vector4',
  /* signature type constants */
  NORMAL: 0x0,
  READ: 0x1,
  SUBTRACT: 0x2,

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
    if(arguments.length == 2) return memoryjs.readMemory(address, dataType.toLowerCase());
    else memoryjs.readMemory(address, dataType.toLowerCase(), callback);
  },

  writeMemory: function(address, value, dataType, callback){
    if (arguments.length == 3) return memoryjs.writeMemory(address, value, dataType.toLowerCase());
    else memoryjs.writeMemory(address, value, dataType.toLowerCase(), callback);
  },

  findPattern: function (moduleName, signature, signatureType, patternOffset, addressOffset, callback) {
    if (arguments.length == 5) return memoryjs.findPattern(moduleName, signature, signatureType, patternOffset, addressOffset);
    else memoryjs.findPattern(moduleName, signature, signatureType, patternOffset, addressOffset, callback);
  }
};
