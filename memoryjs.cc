#include <node.h>
#include <windows.h>
#include <TlHelp32.h>
#include <string>
#include "module.h"
#include "process.h"
#include "memoryjs.h"

//namespace Memory {

using v8::Exception;
using v8::Function;
using v8::FunctionCallbackInfo;
using v8::Isolate;
using v8::Local;
using v8::Object;
using v8::String;
using v8::Number;
using v8::Value;
using v8::Handle;
using v8::Array;

process Process;
module Module;

void memoryjs::throwError(char* error, Isolate* isolate) {
	isolate->ThrowException(
		Exception::TypeError(String::NewFromUtf8(isolate, error))
		);
	return;
}

const char* toCharString(const v8::String::Utf8Value &value) {
  return *value;
}

void openProcess(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();

  // If there is neither 1 nor 2 arguments, throw an error
  if(args.Length() != 1 && args.Length() != 2){
    memoryjs::throwError("requires 1 argument, or 2 arguments if a callback is being used", isolate);
    return;
  }

  // If the argument we've been given is
  // not a string, throw an error
  if(!args[0]->IsString()){
	  memoryjs::throwError("first argument must be a string", isolate);
    return;
  }

  // If there is a second argument and it's not
  // a function, throw an error
  if (args.Length() == 2 && !args[1]->IsFunction()) {
	  memoryjs::throwError("second argument must be a function", isolate);
	  return;
  }

  // Convert from v8 to char with toCharString
  v8::String::Utf8Value processName(args[0]);

  // Opens a process and returns PROCESSENTRY32 class
  PROCESSENTRY32 process = Process.openProcess(toCharString(processName), isolate);

  // In case it failed to open, let's keep retrying
  while(!strcmp(process.szExeFile, "")) {
    process = Process.openProcess(toCharString(processName), isolate);
  };

  // Create a v8 Object (JSON) to store the process information
  Local<Object> processInfo = Object::New(isolate);

  // Set the key/values
  processInfo->Set(String::NewFromUtf8(isolate, "cntThreads"), Number::New(isolate, (int)process.cntThreads));
  processInfo->Set(String::NewFromUtf8(isolate, "cntUsage"), Number::New(isolate, (int)process.cntUsage));
  processInfo->Set(String::NewFromUtf8(isolate, "dwFlags"), Number::New(isolate, (int)process.dwFlags));
  processInfo->Set(String::NewFromUtf8(isolate, "dwSize"), Number::New(isolate, (int)process.dwSize));
  processInfo->Set(String::NewFromUtf8(isolate, "szExeFile"), String::NewFromUtf8(isolate, process.szExeFile));
  processInfo->Set(String::NewFromUtf8(isolate, "th32ProcessID"), Number::New(isolate, (int)process.th32ProcessID));
  processInfo->Set(String::NewFromUtf8(isolate, "th32ParentProcessID"), Number::New(isolate, (int)process.th32ParentProcessID));

  // openProcess can either take one argument or can take
  // two arguments for asychronous use (second argument is the callback)
  if(args.Length() == 2){
    // Callback to let the user handle with the information
    Local<Function> callback = Local<Function>::Cast(args[1]);
    const unsigned argc = 1;
    Local<Value> argv[argc] = { processInfo };
    callback->Call(Null(isolate), argc, argv);
  } else {
	// return JSON
    args.GetReturnValue().Set(processInfo);
  }
}

void closeProcess(const FunctionCallbackInfo<Value>& args){
  Isolate* isolate = args.GetIsolate();
  Process.closeProcess();
}

void getProcesses(const FunctionCallbackInfo<Value>& args) {
	Isolate* isolate = args.GetIsolate();

	// If there is neither 1 nor 2 arguments, throw an error
	if (args.Length() > 1) {
		memoryjs::throwError("requires either 0 arguments or 1 argument if a callback is being used", isolate);
		return;
	}

	// If there is a second argument and it's not
	// a function, throw an error
	if (args.Length() == 1 && !args[0]->IsFunction()) {
		memoryjs::throwError("first argument must be a function", isolate);
		return;
	}

	// processEntries stores PROCESSENTRY32s in a vector
	std::vector<PROCESSENTRY32> processEntries = Process.getProcesses(isolate);

	// Creates v8 array with the size being that of the processEntries vector
	// processes is an array of JavaScript objects
	Handle<Array> processes = Array::New(isolate, processEntries.size());

	// Loop over all processes found
	for (std::vector<PROCESSENTRY32>::size_type i = 0; i != processEntries.size(); i++) {
		// Create a v8 object to store the current process' information
		Local<Object> process = Object::New(isolate);

		// Assign key/values
		process->Set(String::NewFromUtf8(isolate, "cntThreads"), Number::New(isolate, (int)processEntries[i].cntThreads));
		process->Set(String::NewFromUtf8(isolate, "cntUsage"), Number::New(isolate, (int)processEntries[i].cntUsage));
		process->Set(String::NewFromUtf8(isolate, "dwFlags"), Number::New(isolate, (int)processEntries[i].dwFlags));
		process->Set(String::NewFromUtf8(isolate, "dwSize"), Number::New(isolate, (int)processEntries[i].dwSize));
		process->Set(String::NewFromUtf8(isolate, "szExeFile"), String::NewFromUtf8(isolate, processEntries[i].szExeFile));
		process->Set(String::NewFromUtf8(isolate, "th32ProcessID"), Number::New(isolate, (int)processEntries[i].th32ProcessID));
		process->Set(String::NewFromUtf8(isolate, "th32ParentProcessID"), Number::New(isolate, (int)processEntries[i].th32ParentProcessID));

		// Push the object to the array
		processes->Set(i, process);
	}

	// getProcesses can either take no arguments or one argument
	// one argument is for asychronous use (the callback)
	if (args.Length() == 1) {
		// Callback to let the user handle with the information
		Local<Function> callback = Local<Function>::Cast(args[0]);
		const unsigned argc = 1;
		Local<Value> argv[argc] = { processes };
		callback->Call(Null(isolate), argc, argv);
	} else {
		// return JSON
		args.GetReturnValue().Set(processes);
	}
}

void getModules(const FunctionCallbackInfo<Value>& args) {
	Isolate* isolate = args.GetIsolate();

	// If there is neither 1 nor 2 arguments, throw an error
	if (args.Length() != 1 && args.Length() != 2) {
		memoryjs::throwError("requires 1 argument, or 2 arguments if a callback is being used", isolate);
		return;
	}

	// If the argument we've been given is not a number, throw an error
	if (!args[0]->IsNumber()) {
		memoryjs::throwError("first argument must be a number", isolate);
		return;
	}

	// If there is a second argument and it's not a function, throw an error
	if (args.Length() == 2 && !args[1]->IsFunction()) {
		memoryjs::throwError("first argument must be a number, second argument must be a function", isolate);
		return;
	}

	// moduleEntries stores MODULEENTRY32s in a vector
	std::vector<MODULEENTRY32> moduleEntries = Module.getModules(args[0]->NumberValue(), isolate);

	// Creates v8 array with the size being that of the moduleEntries vector
	// modules is an array of JavaScript objects
	Handle<Array> modules = Array::New(isolate, moduleEntries.size());

	// Loop over all modules found
	for (std::vector<MODULEENTRY32>::size_type i = 0; i != moduleEntries.size(); i++) {
		// Create a v8 object to store the current module's information
		Local<Object> module = Object::New(isolate);

		// Assign key/values
		module->Set(String::NewFromUtf8(isolate, "dwSize"), Number::New(isolate, (int)moduleEntries[i].dwSize));
		module->Set(String::NewFromUtf8(isolate, "GlblcntUsage"), Number::New(isolate, (int)moduleEntries[i].GlblcntUsage));
		module->Set(String::NewFromUtf8(isolate, "modBaseAddr"), Number::New(isolate, (int)moduleEntries[i].modBaseAddr));
		module->Set(String::NewFromUtf8(isolate, "modBaseSize"), Number::New(isolate, (int)moduleEntries[i].modBaseSize));
		module->Set(String::NewFromUtf8(isolate, "ProccntUsage"), Number::New(isolate, (int)moduleEntries[i].ProccntUsage));
		module->Set(String::NewFromUtf8(isolate, "szExePath"), String::NewFromUtf8(isolate, moduleEntries[i].szExePath));
		module->Set(String::NewFromUtf8(isolate, "szModule"), String::NewFromUtf8(isolate, moduleEntries[i].szModule));
		module->Set(String::NewFromUtf8(isolate, "th32ModuleID"), Number::New(isolate, (int)moduleEntries[i].th32ModuleID));
		module->Set(String::NewFromUtf8(isolate, "th32ModuleID"), Number::New(isolate, (int)moduleEntries[i].th32ProcessID));

		// Push the object to the array
		modules->Set(i, module);
	}

	// getModules can either take no arguments, one argument or two arguments
	// one/two arguments is for asychronous use (the callback)
	if (args.Length() == 2) {
		// Callback to let the user handle with the information
		Local<Function> callback = Local<Function>::Cast(args[1]);
		const unsigned argc = 1;
		Local<Value> argv[argc] = { modules };
		callback->Call(Null(isolate), argc, argv);
	} else {
		// return JSON
		args.GetReturnValue().Set(modules);
	}
}

void findModule(const FunctionCallbackInfo<Value>& args) {
	Isolate* isolate = args.GetIsolate();

	// If there is neither 1 nor, nor 2, nor 3 arguments, throw an error
	if (args.Length() != 1 && args.Length() != 2 && args.Length() != 3) {
		memoryjs::throwError("requires 2 arguments, or 3 arguments if a callback is being used", isolate);
		return;
	}

	// If the argument we've been given is not a string and the
	// second argument we've been given is not a number, throw an error
	if (!args[0]->IsString() && !args[1]->IsNumber()) {
		memoryjs::throwError("first argument must be a string, second argument must be a number", isolate);
		return;
	}

	// If there is a third argument and it's not a function, throw an error
	if (args.Length() == 3 && !args[2]->IsFunction()) {
		memoryjs::throwError("third argument must be a function", isolate);
		return;
	}

	// Convert from v8 to char with toCharString
	v8::String::Utf8Value moduleName(args[0]);

	// Searches all modules for requested module and returns PROCESSENTRY32 class
	MODULEENTRY32 module = Module.findModule(toCharString(moduleName), args[1]->NumberValue(), isolate);

	// In case it failed to open, let's keep retrying
	while (!strcmp(module.szExePath, "")) {
		module = Module.findModule(toCharString(moduleName), args[1]->NumberValue(), isolate);
	};

	// Create a v8 Object (JSON) to store the process information
	Local<Object> moduleInfo = Object::New(isolate);

	// Set the key/values
	moduleInfo->Set(String::NewFromUtf8(isolate, "dwSize"), Number::New(isolate, (int)module.dwSize));
	moduleInfo->Set(String::NewFromUtf8(isolate, "GlblcntUsage"), Number::New(isolate, (int)module.GlblcntUsage));
	moduleInfo->Set(String::NewFromUtf8(isolate, "modBaseAddr"), Number::New(isolate, (int)module.modBaseAddr));
	moduleInfo->Set(String::NewFromUtf8(isolate, "modBaseSize"), Number::New(isolate, (int)module.modBaseSize));
	moduleInfo->Set(String::NewFromUtf8(isolate, "ProccntUsage"), Number::New(isolate, (int)module.ProccntUsage));
	moduleInfo->Set(String::NewFromUtf8(isolate, "szExePath"), String::NewFromUtf8(isolate, module.szExePath));
	moduleInfo->Set(String::NewFromUtf8(isolate, "szModule"), String::NewFromUtf8(isolate, module.szModule));
	moduleInfo->Set(String::NewFromUtf8(isolate, "th32ModuleID"), Number::New(isolate, (int)module.th32ModuleID));
	moduleInfo->Set(String::NewFromUtf8(isolate, "th32ModuleID"), Number::New(isolate, (int)module.th32ProcessID));

	// findModule can either take one or two arguments,
	// three arguments for asychronous use (third argument is the callback)
	if (args.Length() == 3) {
		// Callback to let the user handle with the information
		Local<Function> callback = Local<Function>::Cast(args[2]);
		const unsigned argc = 1;
		Local<Value> argv[argc] = { moduleInfo };
		callback->Call(Null(isolate), argc, argv);
	} else {
		// return JSON
		args.GetReturnValue().Set(moduleInfo);
	}
}

void init(Local<Object> exports) {
  NODE_SET_METHOD(exports, "openProcess", openProcess);
  NODE_SET_METHOD(exports, "closeProcess", closeProcess);
  NODE_SET_METHOD(exports, "getProcesses", getProcesses);
  NODE_SET_METHOD(exports, "getModules", getModules);
  NODE_SET_METHOD(exports, "findModule", findModule);
}

NODE_MODULE(memoryjs, init)

//}
