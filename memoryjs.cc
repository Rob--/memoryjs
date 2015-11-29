#include <node.h>
#include <windows.h>
#include <TlHelp32.h>
#include <string>
#include "process.h"

namespace Memory {

using v8::Exception;
using v8::Function;
using v8::FunctionCallbackInfo;
using v8::Isolate;
using v8::Local;
using v8::Object;
using v8::String;
using v8::Number;
using v8::Value;

process Memory;

void throwError(char* error, Isolate* isolate){
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

  // Argument is the process name,
  // if this hasn't been provided throw an error
  if(args.Length() != 1 && args.Length() != 2){
    throwError("requires 1 argument, or 2 arguments if a callback is being used", isolate);
    return;
  }

  // If the argument we've been given is
  // not a string, throw an error
  if(!args[0]->IsString()){
    throwError("first argument must be a string", isolate);
    return;
  }

  // Convert from v8 to char with toCharString
  v8::String::Utf8Value processName(args[0]);

  // Opens a process and returns PROCESSENTRY32 class
  PROCESSENTRY32 process = Memory.openProcess(toCharString(processName), isolate);

  // In case it failed to open, let's keep retrying
  while(!strcmp(process.szExeFile, "")) {
    process = Memory.openProcess(toCharString(processName), isolate);
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
  } else if(args.Length() == 1){
	// return JSON
    args.GetReturnValue().Set(processInfo);
  }
}

void closeProcess(const FunctionCallbackInfo<Value>& args){
  Isolate* isolate = args.GetIsolate();
  Memory.closeProcess();
}

void init(Local<Object> exports) {
  NODE_SET_METHOD(exports, "openProcess", openProcess);
  NODE_SET_METHOD(exports, "closeProcess", closeProcess);
}

NODE_MODULE(memoryjs, init)

}
