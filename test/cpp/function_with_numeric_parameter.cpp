/*********************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2015 NAN contributors
 *
 * MIT License <https://github.com/nodejs/nan/blob/master/LICENSE.md>
 ********************************************************************/

#include <nan.h>
#include <nan-check.h>

using namespace Nan;  // NOLINT(build/namespaces)

NAN_METHOD(TestNumericParameter) {
    uint32_t aNumber;
    std::string error;
    v8::Isolate* isolate = info.GetIsolate();
    if (Nan::Check(info)
        .ArgumentsCount(1)
        .Argument(0)
        .IsNumber()
        .Bind(aNumber)
        .Error(&error))
    { 

       info.GetReturnValue().Set(v8::Number::New(isolate, aNumber));
    }
    else
    {
        Nan::ThrowTypeError(error.c_str());
    }
}

NAN_MODULE_INIT(Init) {
    NAN_EXPORT(target, TestNumericParameter);
}

NODE_MODULE(function_with_numeric_parameter, Init)
