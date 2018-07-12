const memoryjs = require('./build/Release/memoryjs');

module.exports = {

  // data type constants
  INT: 'int',
  DWORD: 'dword',
  LONG: 'long',
  FLOAT: 'float',
  DOUBLE: 'double',
  BOOL: 'bool',
  BOOLEAN: 'boolean',
  PTR: 'ptr',
  POINTER: 'pointer',
  STR: 'str',
  STRING: 'string',
  VEC3: 'vec3',
  VECTOR3: 'vector3',
  VEC4: 'vec4',
  VECTOR4: 'vector4',

  // signature type constants
  NORMAL: 0x0,
  READ: 0x1,
  SUBTRACT: 0x2,

  openProcess(processIdentifier, callback) {
    if (arguments.length === 1) {
      return memoryjs.openProcess(processIdentifier);
    }

    memoryjs.openProcess(processIdentifier, callback);
  },

  getProcesses(callback) {
    if (arguments.length === 0) {
      return memoryjs.getProcesses();
    }

    memoryjs.getProcesses(callback);
  },

  findModule(moduleName, processId, callback) {
    if (arguments.length === 2) {
      return memoryjs.findModule(moduleName, processId);
    }
 
    memoryjs.findModule(moduleName, processId, callback);
  },

  getModules(processId, callback) {
    if (arguments.length === 1) {
      return memoryjs.getModules(processId);
    }

    memoryjs.getModules(processId, callback);
  },

  readMemory(handle, address, dataType, callback) {
    if (arguments.length === 2) {
      return memoryjs.readMemory(handle, address, dataType.toLowerCase());
    }

    memoryjs.readMemory(handle, address, dataType.toLowerCase(), callback);
  },

  writeMemory(handle, address, value, dataType, callback) {
    if (dataType === 'str' || dataType === 'string') {
      value = value + '\0'; // add terminator
    }

    if (arguments.length === 4) {
      return memoryjs.writeMemory(handle, address, value, dataType.toLowerCase());
    }

    memoryjs.writeMemory(handle, address, value, dataType.toLowerCase(), callback);
  },

  findPattern(handle, moduleName, signature, signatureType, patternOffset, addressOffset, callback) {
    if (arguments.length === 5) {
      return memoryjs.findPattern(handle, moduleName, signature, signatureType, patternOffset, addressOffset);
    }

    memoryjs.findPattern(handle, moduleName, signature, signatureType, patternOffset, addressOffset, callback);
  },

  closeProcess: memoryjs.closeProcess,
};
