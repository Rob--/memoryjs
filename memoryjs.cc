#include <node.h>
#include <windows.h>
#include <TlHelp32.h>
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
  if(args.Length() != 2){
    throwError("requires 2 arguments", isolate);
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

  // Opens a process and returns the HANDLE cast to an int
  int process = Memory.openProcess(toCharString(processName), isolate);

  // Callback, only passes back the integer
  Local<Function> callback = Local<Function>::Cast(args[1]);
  const unsigned argc = 1;
  Local<Value> argv[argc] = { Number::New(isolate, process) };
  callback->Call(Null(isolate), argc, argv);
}

void closeProcess(const FunctionCallbackInfo<Value>& args){
  Isolate* isolate = args.GetIsolate();

  // Argument is the process (HANLE cast to int),
  // if this hasn't been provided throw an error
  if(args.Length() != 1){
    throwError("requires 1 argument", isolate);
    return;
  }

  // If the argument we've been given is
  // not a string, throw an error
  if(!args[0]->IsNumber()){
    throwError("first argument must be an integer", isolate);
    return;
  }

  Memory.closeProcess(args[0]->NumberValue());
}

void init(Local<Object> exports) {
  NODE_SET_METHOD(exports, "openProcess", openProcess);
  NODE_SET_METHOD(exports, "closeProcess", closeProcess);
}

NODE_MODULE(memoryjs, init)

}
