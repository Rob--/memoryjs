#include <node.h>
#include <windows.h>
#include <TlHelp32.h>
#include "process.h"

namespace Memory {

using v8::Exception;
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
  if(args.Length() != 1){
    throwError("openProcess requires 1 argument (the process name)", isolate);
    return;
  }

  // If the argument we've been given is
  // not a string, throw an error
  if(!args[0]->IsString()){
    throwError("the argument for openProcess needs to be a String (the process name)", isolate);
    return;
  }

  v8::String::Utf8Value processName(args[0]);

  args.GetReturnValue().Set(Memory.openProcess(toCharString(processName), isolate));
}

void init(Local<Object> exports) {
  NODE_SET_METHOD(exports, "openProcess", openProcess);
}

NODE_MODULE(memoryjs, init)

}
