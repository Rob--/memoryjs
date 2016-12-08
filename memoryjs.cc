#include <node.h>
#include <windows.h>
#include <TlHelp32.h>
#include <string>
#include "module.h"
#include "process.h"
#include "memoryjs.h"
#include "memory.h"
#include "pattern.h"

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
using v8::Boolean;

process Process;
module Module;
memory Memory;
pattern Pattern;

struct Vector3 {
	float x, y, z;
};

void memoryjs::throwError(char* error, Isolate* isolate) {
  isolate->ThrowException(
    Exception::TypeError(String::NewFromUtf8(isolate, error))
  );
  return;
}

void openProcess(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();
  
  /* If there is neither 1 nor 2 arguments, throw an error */
  if(args.Length() != 1 && args.Length() != 2){
    memoryjs::throwError("requires 1 argument, or 2 arguments if a callback is being used", isolate);
    return;
  }

  /* If the argument we've been given is
     not a string, throw an error */
  if(!args[0]->IsString()){
	  memoryjs::throwError("first argument must be a string", isolate);
    return;
  }

  /* If there is a second argument and
     it's not a function, throw an error */
  if (args.Length() == 2 && !args[1]->IsFunction()) {
	  memoryjs::throwError("second argument must be a function", isolate);
	  return;
  }
  
  v8::String::Utf8Value processName(args[0]);

  /* Define error message that may be set by
     the function that opens the process */
  char* errorMessage = "";

  /* Opens a process and returns PROCESSENTRY32 class */
  PROCESSENTRY32 process = Process.openProcess((char*) *(processName), &errorMessage);

  /* In case it failed to open, let's keep retrying */
  while(!strcmp(process.szExeFile, "")) {
    process = Process.openProcess((char*) *(processName), &errorMessage);
  };

  /* If an error message was returned from the function that opens the process, throw the error.
     Only throw an error if there is no callback (if there's a callback, the error is passed there). */
  if (strcmp(errorMessage, "") && args.Length() != 2) {
	  memoryjs::throwError(errorMessage, isolate);
	  return;
  }

  /* Create a v8 Object (JSON) to store the process information */
  Local<Object> processInfo = Object::New(isolate);

  /* Set the key/values */
  processInfo->Set(String::NewFromUtf8(isolate, "cntThreads"), Number::New(isolate, (int)process.cntThreads));
  processInfo->Set(String::NewFromUtf8(isolate, "szExeFile"), String::NewFromUtf8(isolate, process.szExeFile));
  processInfo->Set(String::NewFromUtf8(isolate, "th32ProcessID"), Number::New(isolate, (int)process.th32ProcessID));
  processInfo->Set(String::NewFromUtf8(isolate, "th32ParentProcessID"), Number::New(isolate, (int)process.th32ParentProcessID));
  processInfo->Set(String::NewFromUtf8(isolate, "pcPriClassBase"), Number::New(isolate, (int)process.pcPriClassBase));

  /* openProcess can either take one argument or can take
     two arguments for asychronous use (second argument is the callback) */
  if(args.Length() == 2){
    /* Callback to let the user handle with the information */
    Local<Function> callback = Local<Function>::Cast(args[1]);
    const unsigned argc = 2;
    Local<Value> argv[argc] = { String::NewFromUtf8(isolate, errorMessage), processInfo };
    callback->Call(Null(isolate), argc, argv);
  } else {
	/* return JSON */
    args.GetReturnValue().Set(processInfo);
  }
}

void closeProcess(const FunctionCallbackInfo<Value>& args){
  Isolate* isolate = args.GetIsolate();
  Process.closeProcess();
}

void getProcesses(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();

  /* If there is neither 1 nor 2 arguments, throw an error */
  if (args.Length() > 1) {
    memoryjs::throwError("requires either 0 arguments or 1 argument if a callback is being used", isolate);
    return;
  }

  /* If there is a second argument and it's not
     a function, throw an error */
  if (args.Length() == 1 && !args[0]->IsFunction()) {
    memoryjs::throwError("first argument must be a function", isolate);
    return;
  }

  /* Define error message that may be set by the function that gets the processes */
  char* errorMessage = "";

  /* processEntries stores PROCESSENTRY32s in a vector */
  std::vector<PROCESSENTRY32> processEntries = Process.getProcesses(&errorMessage);

  /* If an error message was returned from the function that gets the processes, throw the error.
     Only throw an error if there is no callback (if there's a callback, the error is passed there). */
  if (strcmp(errorMessage, "") && args.Length() != 1) {
    memoryjs::throwError(errorMessage, isolate);
    return;
  }

  /* Creates v8 array with the size being that of the processEntries vector
     processes is an array of JavaScript objects */
  Handle<Array> processes = Array::New(isolate, processEntries.size());

  /* Loop over all processes found */
  for (std::vector<PROCESSENTRY32>::size_type i = 0; i != processEntries.size(); i++) {
    /* Create a v8 object to store the current process' information */
    Local<Object> process = Object::New(isolate);

    /* Assign key/values */
    process->Set(String::NewFromUtf8(isolate, "cntThreads"), Number::New(isolate, (int)processEntries[i].cntThreads));
    process->Set(String::NewFromUtf8(isolate, "szExeFile"), String::NewFromUtf8(isolate, processEntries[i].szExeFile));
    process->Set(String::NewFromUtf8(isolate, "th32ProcessID"), Number::New(isolate, (int)processEntries[i].th32ProcessID));
    process->Set(String::NewFromUtf8(isolate, "th32ParentProcessID"), Number::New(isolate, (int)processEntries[i].th32ParentProcessID));
    process->Set(String::NewFromUtf8(isolate, "pcPriClassBase"), Number::New(isolate, (int)processEntries[i].pcPriClassBase));

    /* Push the object to the array */
    processes->Set(i, process);
  }

  /* getProcesses can either take no arguments or one argument
     one argument is for asychronous use (the callback) */
  if (args.Length() == 1) {
    /* Callback to let the user handle with the information */
    Local<Function> callback = Local<Function>::Cast(args[0]);
    const unsigned argc = 2;
    Local<Value> argv[argc] = { String::NewFromUtf8(isolate, errorMessage), processes };
    callback->Call(Null(isolate), argc, argv);
  } else {
    /* return JSON */
    args.GetReturnValue().Set(processes);
  }
}

void getModules(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();

  /* If there is neither 1 nor 2 arguments, throw an error */
  if (args.Length() != 1 && args.Length() != 2) {
    memoryjs::throwError("requires 1 argument, or 2 arguments if a callback is being used", isolate);
    return;
  }

  /* If the argument we've been given is not a number, throw an error */
  if (!args[0]->IsNumber()) {
    memoryjs::throwError("first argument must be a number", isolate);
    return;
  }

  /* If there is a second argument and it's not a function, throw an error */
  if (args.Length() == 2 && !args[1]->IsFunction()) {
    memoryjs::throwError("first argument must be a number, second argument must be a function", isolate);
    return;
  }

  /* Define error message that may be set by the function that gets the modules */
  char* errorMessage = "";

  /* moduleEntries stores MODULEENTRY32s in a vector */
  std::vector<MODULEENTRY32> moduleEntries = Module.getModules(args[0]->Int32Value(), &errorMessage);

  /* If an error message was returned from the function getting the modules, throw the error.
     Only throw an error if there is no callback (if there's a callback, the error is passed there). */
  if (strcmp(errorMessage, "") && args.Length() != 2) {
    memoryjs::throwError(errorMessage, isolate);
    return;
  }

  /* Creates v8 array with the size being that of the moduleEntries vector
     modules is an array of JavaScript objects */
  Handle<Array> modules = Array::New(isolate, moduleEntries.size());

  /* Loop over all modules found */
  for (std::vector<MODULEENTRY32>::size_type i = 0; i != moduleEntries.size(); i++) {
    /* Create a v8 object to store the current module's information */
    Local<Object> module = Object::New(isolate);

    /* Assign key/values */
    module->Set(String::NewFromUtf8(isolate, "modBaseAddr"), Number::New(isolate, (int)moduleEntries[i].modBaseAddr));
    module->Set(String::NewFromUtf8(isolate, "modBaseSize"), Number::New(isolate, (int)moduleEntries[i].modBaseSize));
    module->Set(String::NewFromUtf8(isolate, "szExePath"), String::NewFromUtf8(isolate, moduleEntries[i].szExePath));
    module->Set(String::NewFromUtf8(isolate, "szModule"), String::NewFromUtf8(isolate, moduleEntries[i].szModule));
    module->Set(String::NewFromUtf8(isolate, "th32ModuleID"), Number::New(isolate, (int)moduleEntries[i].th32ProcessID));

    /* Push the object to the array */
    modules->Set(i, module);
  }

  /* getModules can either take one argument or two arguments
     one/two arguments is for asychronous use (the callback) */
  if (args.Length() == 2) {
    /* Callback to let the user handle with the information */
    Local<Function> callback = Local<Function>::Cast(args[1]);
    const unsigned argc = 2;
    Local<Value> argv[argc] = { String::NewFromUtf8(isolate, errorMessage), modules };
    callback->Call(Null(isolate), argc, argv);
  } else {
    /* return JSON */
    args.GetReturnValue().Set(modules);
  }
}

void findModule(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();

  /* If there is neither 1 nor, nor 2, nor 3 arguments, throw an error */
  if (args.Length() != 1 && args.Length() != 2 && args.Length() != 3) {
    memoryjs::throwError("requires 1 argument, 2 arguments, or 3 arguments if a callback is being used", isolate);
    return;
  }

  /* If the argument we've been given is not a string and the
     second argument we've been given is not a number, throw an error */
  if (!args[0]->IsString() && !args[1]->IsNumber()) {
    memoryjs::throwError("first argument must be a string, second argument must be a number", isolate);
    return;
  }

  /* If there is a third argument and it's not a function, throw an error */
  if (args.Length() == 3 && !args[2]->IsFunction()) {
    memoryjs::throwError("third argument must be a function", isolate);
    return;
  }
	
  v8::String::Utf8Value moduleName(args[0]);
	
  /* Define error message that may be set by the function that gets the modules */
  char* errorMessage = "";

  /* Searches all modules for requested module and returns PROCESSENTRY32 class */
  MODULEENTRY32 module = Module.findModule((char*) *(moduleName), args[1]->Int32Value(), &errorMessage);

  /* If an error message was returned from the function getting the module, throw the error.
     Only throw an error if there is no callback (if there's a callback, the error is passed there). */
  if (strcmp(errorMessage, "") && args.Length() != 3) {
    memoryjs::throwError(errorMessage, isolate);
    return;
  }

  /* In case it failed to open, let's keep retrying */
  while (!strcmp(module.szExePath, "")) {
    module = Module.findModule((char*) *(moduleName), args[1]->Int32Value(), &errorMessage);
  };

  /* Create a v8 Object (JSON) to store the process information */
  Local<Object> moduleInfo = Object::New(isolate);

  /* Set the key/values */
  moduleInfo->Set(String::NewFromUtf8(isolate, "modBaseAddr"), Number::New(isolate, (int)module.modBaseAddr));
  moduleInfo->Set(String::NewFromUtf8(isolate, "modBaseSize"), Number::New(isolate, (int)module.modBaseSize));
  moduleInfo->Set(String::NewFromUtf8(isolate, "szExePath"), String::NewFromUtf8(isolate, module.szExePath));
  moduleInfo->Set(String::NewFromUtf8(isolate, "szModule"), String::NewFromUtf8(isolate, module.szModule));
  moduleInfo->Set(String::NewFromUtf8(isolate, "th32ProcessID"), Number::New(isolate, (int)module.th32ProcessID));
  moduleInfo->Set(String::NewFromUtf8(isolate, "hModule"), Number::New(isolate, (int)module.hModule));

  /* findModule can either take one or two arguments,
     three arguments for asychronous use (third argument is the callback) */
  if (args.Length() == 3) {
    /* Callback to let the user handle with the information */
    Local<Function> callback = Local<Function>::Cast(args[2]);
    const unsigned argc = 2;
    Local<Value> argv[argc] = { String::NewFromUtf8(isolate, errorMessage), moduleInfo };
    callback->Call(Null(isolate), argc, argv);
  } else {
    /* return JSON */
    args.GetReturnValue().Set(moduleInfo);
  }
}

void readMemory(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();

  /* If there is neither 2, nor 3 arguments, throw an error */
  if (args.Length() != 2 && args.Length() != 3) {
    memoryjs::throwError("requires 2 arguments, or 3 arguments if a callback is being used", isolate);
    return;
  }

  /* If the argument we've been given is not a number and the
     second argument we've been given is not a string, throw an error */
  if (!args[0]->IsNumber() && !args[1]->IsString()) {
    memoryjs::throwError("first argument must be a number, second argument must be a string", isolate);
    return;
  }

  /* If there is a third argument and it's not a function, throw an error */
  if (args.Length() == 3 && !args[2]->IsFunction()) {
    memoryjs::throwError("third argument must be a function", isolate);
    return;
  }

  v8::String::Utf8Value dataTypeArg(args[1]);
  char* dataType = (char*) *(dataTypeArg);

  /* Set callback variables in the case the a callback parameter has been passed */
  Local<Function> callback = Local<Function>::Cast(args[2]);
  const unsigned argc = 2;
  Local<Value> argv[argc];

  /* Define the error message that will be set if no data type is recognised */
  argv[1] = String::NewFromUtf8(isolate, "");

  /* following if statements find the data type to read and then return the correct data type
     args[0] -> Uint32Value() is the address to read, unsigned int is used because address needs to be positive */
  if (!strcmp(dataType, "int")) {

    int result = Memory.readMemory<int>(process::hProcess, args[0]->Uint32Value());
    if (args.Length() == 3) argv[0] = Number::New(isolate, result);
    else args.GetReturnValue().Set(Number::New(isolate, result));

  } else if (!strcmp(dataType, "dword")) {

    DWORD result = Memory.readMemory<DWORD>(process::hProcess, args[0]->Uint32Value());
    if (args.Length() == 3) argv[0] = Number::New(isolate, result);
    else args.GetReturnValue().Set(Number::New(isolate, result));

  } else if (!strcmp(dataType, "long")) {

    long result = Memory.readMemory<long>(process::hProcess, args[0]->Uint32Value());
    if (args.Length() == 3) argv[0] = Number::New(isolate, result);
    else args.GetReturnValue().Set(Number::New(isolate, result));

  } else if (!strcmp(dataType, "float")) {

    float result = Memory.readMemory<float>(process::hProcess, args[0]->Uint32Value());
    if (args.Length() == 3) argv[0] = Number::New(isolate, result);
    else args.GetReturnValue().Set(Number::New(isolate, result));

	} else if (!strcmp(dataType, "double")) {
		
    double result = Memory.readMemory<double>(process::hProcess, args[0]->Uint32Value());
    if (args.Length() == 3) argv[0] = Number::New(isolate, result);
    else args.GetReturnValue().Set(Number::New(isolate, result));

  } else if (!strcmp(dataType, "bool") || !strcmp(dataType, "boolean")) {

    bool result = Memory.readMemory<bool>(process::hProcess, args[0]->Uint32Value());
    if (args.Length() == 3) argv[0] = Boolean::New(isolate, result);
    else args.GetReturnValue().Set(Boolean::New(isolate, result));

  }
  else if (!strcmp(dataType, "string") || !strcmp(dataType, "str")) {

	  char* result = Memory.readMemory<char*>(process::hProcess, args[0]->Uint32Value());
	  if (args.Length() == 3) argv[0] = String::NewFromUtf8(isolate, result);
	  else args.GetReturnValue().Set(String::NewFromUtf8(isolate, result));

  } else if (!strcmp(dataType, "vector3") || !strcmp(dataType, "vec3")) {

	  Vector3 result = Memory.readMemory<Vector3>(process::hProcess, args[0]->Uint32Value());
	  Local<Object> moduleInfo = Object::New(isolate);
	  moduleInfo->Set(String::NewFromUtf8(isolate, "x"), Number::New(isolate, result.x));
	  moduleInfo->Set(String::NewFromUtf8(isolate, "y"), Number::New(isolate, result.y));
	  moduleInfo->Set(String::NewFromUtf8(isolate, "z"), Number::New(isolate, result.z));
	  if (args.Length() == 3) argv[0] = moduleInfo;
	  else args.GetReturnValue().Set(moduleInfo);

  } else {

    if (args.Length() == 3) argv[1] = String::NewFromUtf8(isolate, "unexpected data type");
    else return memoryjs::throwError("unexpected data type", isolate);

  }

  /* We check if there is three arguments and if the third argument is a function earlier on
     now we check again if we must call the function passed on */
  if (args.Length() == 3) callback->Call(Null(isolate), argc, argv);
}

void writeMemory(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();

  /* If there is not 3 arguments, throw an error */
  if (args.Length() != 3) {
    memoryjs::throwError("requires 3 arguments", isolate);
    return;
  }

  /* If the argument we've been given is not a number and the
     third argument we've been given is not a string, throw an error */
  if (!args[0]->IsNumber() && !args[2]->IsString()) {
    memoryjs::throwError("first argument must be a number, second argument must be a string", isolate);
    return;
  }

  v8::String::Utf8Value dataTypeArg(args[2]);
  char* dataType = (char*)*(dataTypeArg);

  /* Set callback variables in the case the a callback parameter has been passed */
  Local<Function> callback = Local<Function>::Cast(args[2]);
  const unsigned argc = 1;
  Local<Value> argv[argc];

  /* Define the error message that will be set if no data type is recognised */
  argv[0] = String::NewFromUtf8(isolate, "");

  /* following if statements find the data type to read and then return the correct data type
     args[0] -> Uint32Value() is the address to read, unsigned int is used because address needs to be positive
     args[2] -> value is the value to write to the address */
  if (!strcmp(dataType, "int")) {

    Memory.writeMemory<int>(process::hProcess, args[0]->Uint32Value(), args[1]->NumberValue());

  } else if (!strcmp(dataType, "dword")) {

    Memory.writeMemory<DWORD>(process::hProcess, args[0]->Uint32Value(), args[1]->NumberValue());

  } else if (!strcmp(dataType, "long")) {

    Memory.writeMemory<long>(process::hProcess, args[0]->Uint32Value(), args[1]->NumberValue());

  } else if (!strcmp(dataType, "float")) {

    Memory.writeMemory<float>(process::hProcess, args[0]->Uint32Value(), args[1]->NumberValue());

  } else if (!strcmp(dataType, "double")) {

    Memory.writeMemory<double>(process::hProcess, args[0]->Uint32Value(), args[1]->NumberValue());

  } else if (!strcmp(dataType, "bool") || !strcmp(dataType, "boolean")) {

    Memory.writeMemory<bool>(process::hProcess, args[0]->Uint32Value(), args[1]->BooleanValue());

  } else if (!strcmp(dataType, "string") || !strcmp(dataType, "str")) {

	  v8::String::Utf8Value valueParam(args[1]->ToString());
	  Memory.writeMemory<std::string>(process::hProcess, args[0]->Uint32Value(), std::string(*valueParam));

  } else if (!strcmp(dataType, "vector3") || !strcmp(dataType, "vec3")) {

	  Handle<Object> value = Handle<Object>::Cast(args[1]);
	  Vector3 vector = {
		  value->Get(String::NewFromUtf8(isolate, "x"))->NumberValue(),
		  value->Get(String::NewFromUtf8(isolate, "y"))->NumberValue(),
		  value->Get(String::NewFromUtf8(isolate, "z"))->NumberValue()
	  };
	  Memory.writeMemory<Vector3>(process::hProcess, args[0]->Uint32Value(), vector);

  } else {

    if (args.Length() == 1) argv[0] = String::NewFromUtf8(isolate, "unexpected data type");
    else return memoryjs::throwError("unexpected data type", isolate);

  }

  /* If there is a callback, return the error message (blank if no error) */
  if (args.Length() == 1) callback->Call(Null(isolate), argc, argv);
}

void findPattern(const FunctionCallbackInfo<Value>& args) {
	Isolate* isolate = args.GetIsolate();

	/* If there is not 5 arguments, or 6 incl. callback, throw an error */
	if (args.Length() != 5 && args.Length() != 6) {
		memoryjs::throwError("requires 5 arguments, or 6 arguments if a callback is being used", isolate);
		return;
	}

	/* If the argument we've been given is not a string and the
	third argument we've been given is not a string, throw an error */
	if (!args[0]->IsNumber() && !args[1]->IsString() && !args[2]->IsNumber() && !args[3]->IsNumber() && !args[4]->IsNumber()) {
		memoryjs::throwError("first argument must be a number, the remaining arguments must be numbers apart from the callback", isolate);
		return;
	}

	/* If there is a sixth argument and it's not a function, throw an error */
	if (args.Length() == 6 && !args[5]->IsFunction()) {
		memoryjs::throwError("sixth argument must be a function", isolate);
		return;
	}

	/* Address of findPattern result */
	uintptr_t address = -1;

	for (std::vector<MODULEENTRY32>::size_type i = 0; i != Module.moduleEntries.size(); i++) {

		/* Convert the module name to C++ string to find the module we want */
		v8::String::Utf8Value moduleName(args[0]);
		if (!strcmp(Module.moduleEntries[i].szModule, std::string(*moduleName).c_str())) {

			/* Convert the signature to a C++ string */
			v8::String::Utf8Value signature(args[1]->ToString());

			/* Pattern scan for the address */
			address = Pattern.findPattern(Module.moduleEntries[i], std::string(*signature).c_str(), args[2]->Uint32Value(), args[3]->Uint32Value(), args[4]->Uint32Value());
			break;
		}
	}

	char* errorMessage = "";

	/* If address is still the value we set it as, it probably means we couldn't find the module */
	if (address == -1) errorMessage = "unable to find module";
	/* If it's -2 this means there was no match to the pattern */
	if (address == -2) errorMessage = "no match found";

	/* findPattern can be asynchronous */
	if (args.Length() == 6) {
		/* Callback to let the user handle with the information */
		Local<Function> callback = Local<Function>::Cast(args[5]);
		const unsigned argc = 2;
		Local<Value> argv[argc] = { String::NewFromUtf8(isolate, errorMessage), Number::New(isolate, address) };
		callback->Call(Null(isolate), argc, argv);
	}
	else {
		/* return JSON */
		args.GetReturnValue().Set(Number::New(isolate, address));
	}
}

void init(Local<Object> exports) {
  NODE_SET_METHOD(exports, "openProcess", openProcess);
  NODE_SET_METHOD(exports, "closeProcess", closeProcess);
  NODE_SET_METHOD(exports, "getProcesses", getProcesses);
  NODE_SET_METHOD(exports, "getModules", getModules);
  NODE_SET_METHOD(exports, "findModule", findModule);
  NODE_SET_METHOD(exports, "readMemory", readMemory);
  NODE_SET_METHOD(exports, "writeMemory", writeMemory);
  NODE_SET_METHOD(exports, "findPattern", findPattern);
}

NODE_MODULE(memoryjs, init)
