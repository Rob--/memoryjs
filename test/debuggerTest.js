const memoryjs = require('../index');
const processName = 'Testing Things.exe';

const processObject = memoryjs.openProcess(processName);
const processId = processObject.th32ProcessID;

// Address of variable
const address = 0xEFFBF0;

// When should we breakpoint? On read, write or execute
const trigger = memoryjs.TRIGGER_ACCESS;
const dataType = memoryjs.INT;

// Whether to end the process once debugging has finished
const killOnDetatch = false;

/**
 * Example 1: Using the `Debugger` wrapper class.
 * The library contanis a wrapper class for hardware debugging.
 * It works by simply registering a hardware breakpoint and
 * then listening for all debug events that are emitted when
 * a breakpoint occurs.
 */

const hardwareDebugger = memoryjs.Debugger;

// Attach the debugger to the process
hardwareDebugger.attach(processId, killOnDetatch);

const registerUsed = hardwareDebugger.setHardwareBreakpoint(processId, address, trigger, dataType);

// `debugEvent` event emission catches debug events from all registers
hardwareDebugger.on('debugEvent', ({ register, event }) => {
  console.log(`Hardware Register ${register} breakpoint`);
  console.log(event);
});

// You can listen to debug events from specific hardware registers
// by listening to whatever register was returned from `setHardwareBreakpoint`
hardwareDebugger.on(registerUsed, (event) => {
  console.log(event);
});

// Don't forget to call `hardwareDebugger.detatch()` when you're done!

/**
 * Example 2: Manually using the exposed functions
 * There are a few steps involved when not using the wrapper:
 *
 * 1. Attatch the debugger
 * 2. Set your hardware breakpoints by manually referencing
 *    which register you want to set. It's important you keep
 *    track of which hardware registers you use as there are only 4
 *    meaning only 4 breakpoints can be set.
 *    You also need to manually reference the size of the data type.
 * 3. Constantly call `awaitDebugEvent` to wait for debug events
 * 4. When a debug event occurs, call `handleDebugEvent`
 * 5. Don't forget to detatch the debugger via `memoryjs.detatch(processId)`
 */

memoryjs.attachDebugger(processId, killOnDetatch);

// There are 4 hardware registers:
// `memoryjs.DR0` through `memoryjs.DR3`
const registerToUse = memoryjs.DR0;

// Our `address` references an integer variable. An integer
// is 4 bytes therefore we pass `4` to the `size` parameter.
const size = 4;
memoryjs.setHardwareBreakpoint(processId, address, registerToUse, trigger, size);

// How long to wait for the debug event before timing out
const timeout = 100;

// The interval duration must be the same or larger than the `timeout` value.
// `awaitDebugEvent` works by waiting a certain amount of time before timing out,
// therefore we only want to call the method again when we're sure the previous
// call has already timed out.
setInterval(() => {
  // `debugEvent` can be null if no event occurred
  const debugEvent = memoryjs.awaitDebugEvent(registerToUse, timeout);

  // If a breakpoint occurred, handle it
  if (debugEvent) {
    memoryjs.handleDebugEvent(debugEvent.processId, debugEvent.threadId);
  }
}, timeout);

// Don't forget to detatch the debugger!
// memoryjs.detatchDebugger(processId);

memoryjs.closeProcess(processObject.handle);
