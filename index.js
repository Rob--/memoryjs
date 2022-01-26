const fs = require('fs');
const memoryjs = require('./build/Release/memoryjs');
const DebuggerClass = require('./src/debugger');
const constants = require('./src/consts');
const { STRUCTRON_TYPE_STRING } = require('./src/utils');

const Debugger = new DebuggerClass(memoryjs);

const library = {
  ...constants,

  openProcess(processIdentifier, callback) {
    if (arguments.length === 1) {
      return memoryjs.openProcess(processIdentifier);
    }

    return memoryjs.openProcess(processIdentifier, callback);
  },

  closeProcess(handle) {
    return memoryjs.closeProcess(handle);
  },

  getProcesses(callback) {
    if (arguments.length === 0) {
      return memoryjs.getProcesses();
    }

    return memoryjs.getProcesses(callback);
  },

  findModule(moduleName, processId, callback) {
    if (arguments.length === 2) {
      return memoryjs.findModule(moduleName, processId);
    }

    return memoryjs.findModule(moduleName, processId, callback);
  },

  getModules(processId, callback) {
    if (arguments.length === 1) {
      return memoryjs.getModules(processId);
    }

    return memoryjs.getModules(processId, callback);
  },

  readMemory(handle, address, dataType, callback) {
    if (arguments.length === 3) {
      return memoryjs.readMemory(handle, address, dataType.toLowerCase());
    }

    return memoryjs.readMemory(handle, address, dataType.toLowerCase(), callback);
  },

  readBuffer(handle, address, size, callback) {
    if (arguments.length === 3) {
      return memoryjs.readBuffer(handle, address, size);
    }

    return memoryjs.readBuffer(handle, address, size, callback);
  },

  writeMemory(handle, address, value, dataType) {
    let dataValue = value;
    if (dataType === constants.STR || dataType === constants.STRING) {
      dataValue += '\0'; // add terminator
    }

    return memoryjs.writeMemory(handle, address, dataValue, dataType.toLowerCase());
  },

  writeBuffer(handle, address, buffer) {
    return memoryjs.writeBuffer(handle, address, buffer);
  },

  // eslint-disable-next-line max-len
  findPattern() {
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
  },

  callFunction(handle, args, returnType, address, callback) {
    if (arguments.length === 4) {
      return memoryjs.callFunction(handle, args, returnType, address);
    }

    return memoryjs.callFunction(handle, args, returnType, address, callback);
  },

  virtualAllocEx(handle, address, size, allocationType, protection, callback) {
    if (arguments.length === 5) {
      return memoryjs.virtualAllocEx(handle, address, size, allocationType, protection);
    }

    return memoryjs.virtualAllocEx(handle, address, size, allocationType, protection, callback);
  },

  virtualProtectEx(handle, address, size, protection, callback) {
    if (arguments.length === 4) {
      return memoryjs.virtualProtectEx(handle, address, size, protection);
    }

    return memoryjs.virtualProtectEx(handle, address, size, protection, callback);
  },

  getRegions(handle, getOffsets, callback) {
    if (arguments.length === 1) {
      return memoryjs.getRegions(handle);
    }

    return memoryjs.getRegions(handle, callback);
  },

  virtualQueryEx(handle, address, callback) {
    if (arguments.length === 2) {
      return memoryjs.virtualQueryEx(handle, address);
    }

    return memoryjs.virtualQueryEx(handle, address, callback);
  },

  attachDebugger: memoryjs.attachDebugger,
  detatchDebugger: memoryjs.detatchDebugger,
  awaitDebugEvent: memoryjs.awaitDebugEvent,
  handleDebugEvent: memoryjs.handleDebugEvent,
  setHardwareBreakpoint: memoryjs.setHardwareBreakpoint,
  removeHardwareBreakpoint: memoryjs.removeHardwareBreakpoint,

  injectDll(handle, dllPath, callback) {
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
  },

  unloadDll(handle, module, callback) {
    if (arguments.length === 2) {
      return memoryjs.unloadDll(handle, module);
    }

    return memoryjs.unloadDll(handle, module, callback);
  },

  getContext(processId, address, trigger, callback) {
    Debugger.attach(processId);
    const register = Debugger.setHardwareBreakpoint(processId, address, trigger, constants.BYTE);

    const debugEvent = memoryjs.awaitDebugEvent(register, 1000);

    if (debugEvent) {
      memoryjs.handleDebugEvent(debugEvent.processId, debugEvent.threadId);
      Debugger.detach(processId);

      if (typeof callback === 'function') {
        callback(debugEvent.context);
        return;
      }

      return debugEvent.context;
    }
  },

  Debugger,
};

module.exports = {
  ...library,
  STRUCTRON_TYPE_STRING: STRUCTRON_TYPE_STRING(library),
};
