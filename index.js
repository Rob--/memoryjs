const fs = require('fs');
const memoryjs = require('./build/Release/memoryjs');
const Debugger = require('./src/debugger');
const constants = require('./src/consts');
const { STRUCTRON_TYPE_STRING } = require('./src/utils');

function openProcess(processIdentifier, callback) {
  if (arguments.length === 1) {
    return memoryjs.openProcess(processIdentifier);
  }

  return memoryjs.openProcess(processIdentifier, callback);
}

function closeProcess(handle) {
  return memoryjs.closeProcess(handle);
}

function getProcesses(callback) {
  if (arguments.length === 0) {
    return memoryjs.getProcesses();
  }

  return memoryjs.getProcesses(callback);
}

function findModule(moduleName, processId, callback) {
  if (arguments.length === 2) {
    return memoryjs.findModule(moduleName, processId);
  }

  return memoryjs.findModule(moduleName, processId, callback);
}

function getModules(processId, callback) {
  if (arguments.length === 1) {
    return memoryjs.getModules(processId);
  }

  return memoryjs.getModules(processId, callback);
}

function readMemory(handle, address, dataType, callback) {
  if (dataType.toLowerCase().endsWith('_be')) {
    return readMemoryBE(handle, address, dataType, callback);
  }

  if (arguments.length === 3) {
    return memoryjs.readMemory(handle, address, dataType.toLowerCase());
  }

  return memoryjs.readMemory(handle, address, dataType.toLowerCase(), callback);
}

function readMemoryBE(handle, address, dataType, callback) {
  let value = null;

  switch (dataType) {
    case constants.INT64_BE:
      value = readBuffer(handle, address, 8).readBigInt64BE();
      break;

    case constants.UINT64_BE:
      value = readBuffer(handle, address, 8).readBigUInt64BE();
      break;

    case constants.INT32_BE:
    case constants.INT_BE:
    case constants.LONG_BE:
      value = readBuffer(handle, address, 4).readInt32BE();
      break;

    case constants.UINT32_BE:
    case constants.UINT_BE:
    case constants.ULONG_BE:
      value = readBuffer(handle, address, 4).readUInt32BE();
      break;

    case constants.INT16_BE:
    case constants.SHORT_BE:
      value = readBuffer(handle, address, 2).readInt16BE();
      break;

    case constants.UINT16_BE:
    case constants.USHORT_BE:
      value = readBuffer(handle, address, 2).readUInt16BE();
      break;

    case constants.FLOAT_BE:
      value = readBuffer(handle, address, 4).readFloatBE();
      break;

    case constants.DOUBLE_BE:
      value = readBuffer(handle, address, 8).readDoubleBE();
      break;
  }

  if (typeof callback !== 'function') {
    if (value === null) {
      throw new Error('Invalid data type argument!');
    }

    return value;
  }

  callback(value === null ? 'Invalid data type argument!' : '', value);
}

function readBuffer(handle, address, size, callback) {
  if (arguments.length === 3) {
    return memoryjs.readBuffer(handle, address, size);
  }

  return memoryjs.readBuffer(handle, address, size, callback);
}

function writeMemory(handle, address, value, dataType) {
  let dataValue = value;
  if (dataType === constants.STR || dataType === constants.STRING) {
    dataValue += '\0'; // add terminator
  }

  const bigintTypes = [constants.INT64, constants.INT64_BE, constants.UINT64, constants.UINT64_BE];
  if (bigintTypes.indexOf(dataType) != -1 && typeof value !== 'bigint') {
    throw new Error(`${dataType.toUpperCase()} expects type BigInt`);
  }

  if (dataType.endsWith('_be')) {
    return writeMemoryBE(handle, address, dataValue, dataType);
  }

  return memoryjs.writeMemory(handle, address, dataValue, dataType.toLowerCase());
}

function writeMemoryBE(handle, address, value, dataType) {
  let buffer = null;

  switch (dataType) {
    case constants.INT64_BE:
      if (typeof value !== 'bigint') {
        throw new Error('INT64_BE expects type BigInt');
      }
      buffer = Buffer.alloc(8);
      buffer.writeBigInt64BE(value);
      break;

    case constants.UINT64_BE:
      if (typeof value !== 'bigint') {
        throw new Error('UINT64_BE expects type BigInt');
      }
      buffer = Buffer.alloc(8);
      buffer.writeBigUInt64BE(value);
      break;

    case constants.INT32_BE:
    case constants.INT_BE:
    case constants.LONG_BE:
      buffer = Buffer.alloc(4);
      buffer.writeInt32BE(value);
      break;

    case constants.UINT32_BE:
    case constants.UINT_BE:
    case constants.ULONG_BE:
      buffer = Buffer.alloc(4);
      buffer.writeUInt32BE(value);
      break;

    case constants.INT16_BE:
    case constants.SHORT_BE:
      buffer = Buffer.alloc(2);
      buffer.writeInt16BE(value);
      break;

    case constants.UINT16_BE:
    case constants.USHORT_BE:
      buffer = Buffer.alloc(2);
      buffer.writeUInt16BE(value);
      break;

    case constants.FLOAT_BE:
      buffer = Buffer.alloc(4);
      buffer.writeFloatBE(value);
      break;

    case constants.DOUBLE_BE:
      buffer = Buffer.alloc(8);
      buffer.writeDoubleBE(value);
      break;
  }

  if (buffer == null) {
    throw new Error('Invalid data type argument!');
  }

  writeBuffer(handle, address, buffer);
}

function writeBuffer(handle, address, buffer) {
  return memoryjs.writeBuffer(handle, address, buffer);
}

function findPattern() {
  const findPattern           = ['number', 'string', 'number', 'number'].toString();
  const findPatternByModule   = ['number', 'string', 'string', 'number', 'number'].toString();
  const findPatternByAddress  = ['number', 'number', 'string', 'number', 'number'].toString();

  const args = Array.from(arguments).map(arg => typeof arg);

  if (args.slice(0, 4).toString() === findPattern) {
    if (args.length === 4 || (args.length === 5 && args[4] === 'function')) {
      return memoryjs.findPattern(...arguments);
    }
  }

  if (args.slice(0, 5).toString() === findPatternByModule) {
    if (args.length === 5 || (args.length === 6 && args[5] === 'function')) {
      return memoryjs.findPatternByModule(...arguments);
    }
  }

  if (args.slice(0, 5).toString() === findPatternByAddress) {
    if (args.length === 5 || (args.length === 6 && args[5] === 'function')) {
      return memoryjs.findPatternByAddress(...arguments);
    }
  }

  throw new Error('invalid arguments!');
}

function callFunction(handle, args, returnType, address, callback) {
  if (arguments.length === 4) {
    return memoryjs.callFunction(handle, args, returnType, address);
  }

  return memoryjs.callFunction(handle, args, returnType, address, callback);
}

function virtualAllocEx(handle, address, size, allocationType, protection, callback) {
  if (arguments.length === 5) {
    return memoryjs.virtualAllocEx(handle, address, size, allocationType, protection);
  }

  return memoryjs.virtualAllocEx(handle, address, size, allocationType, protection, callback);
}

function virtualProtectEx(handle, address, size, protection, callback) {
  if (arguments.length === 4) {
    return memoryjs.virtualProtectEx(handle, address, size, protection);
  }

  return memoryjs.virtualProtectEx(handle, address, size, protection, callback);
}

function getRegions(handle, getOffsets, callback) {
  if (arguments.length === 1) {
    return memoryjs.getRegions(handle);
  }

  return memoryjs.getRegions(handle, callback);
}

function virtualQueryEx(handle, address, callback) {
  if (arguments.length === 2) {
    return memoryjs.virtualQueryEx(handle, address);
  }

  return memoryjs.virtualQueryEx(handle, address, callback);
}

function injectDll(handle, dllPath, callback) {
  if (!dllPath.endsWith('.dll')) {
    throw new Error("Given path is invalid: file is not of type 'dll'.");
  }

  if (!fs.existsSync(dllPath)) {
    throw new Error('Given path is invaild: file does not exist.');
  }

  if (arguments.length === 2) {
    return memoryjs.injectDll(handle, dllPath);
  }

  return memoryjs.injectDll(handle, dllPath, callback);
}

function unloadDll(handle, module, callback) {
  if (arguments.length === 2) {
    return memoryjs.unloadDll(handle, module);
  }

  return memoryjs.unloadDll(handle, module, callback);
}

const library = {
  openProcess,
  closeProcess,
  getProcesses,
  findModule,
  getModules,
  readMemory,
  readBuffer,
  writeMemory,
  writeBuffer,
  findPattern,
  callFunction,
  virtualAllocEx,
  virtualProtectEx,
  getRegions,
  virtualQueryEx,
  injectDll,
  unloadDll,
  attachDebugger: memoryjs.attachDebugger,
  detachDebugger: memoryjs.detachDebugger,
  awaitDebugEvent: memoryjs.awaitDebugEvent,
  handleDebugEvent: memoryjs.handleDebugEvent,
  setHardwareBreakpoint: memoryjs.setHardwareBreakpoint,
  removeHardwareBreakpoint: memoryjs.removeHardwareBreakpoint,
  Debugger: new Debugger(memoryjs),
};

module.exports = {
  ...constants,
  ...library,
  STRUCTRON_TYPE_STRING: STRUCTRON_TYPE_STRING(library),
};
