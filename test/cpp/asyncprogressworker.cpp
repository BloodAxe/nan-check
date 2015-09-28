/*********************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2015 NAN contributors
 *
 * MIT License <https://github.com/nodejs/nan/blob/master/LICENSE.md>
 ********************************************************************/

#ifndef _WIN32
#include <unistd.h>
#define Sleep(x) usleep((x)*1000)
#endif
#include <nan.h>
#include <nan-check.h>

using namespace Nan;  // NOLINT(build/namespaces)

class ProgressWorker : public AsyncProgressWorker {
 public:
  ProgressWorker(
      Callback *callback
    , Callback *progress
    , int milliseconds
    , int iters)
    : AsyncProgressWorker(callback), progress(progress)
    , milliseconds(milliseconds), iters(iters) {}
  ~ProgressWorker() {}

  void Execute (const AsyncProgressWorker::ExecutionProgress& progress) {
    for (int i = 0; i < iters; ++i) {
      progress.Send(reinterpret_cast<const char*>(&i), sizeof(int));
      Sleep(milliseconds);
    }
  }

  void HandleProgressCallback(const char *data, size_t size) {
    HandleScope scope;

    v8::Local<v8::Value> argv[] = {
        New<v8::Integer>(*reinterpret_cast<int*>(const_cast<char*>(data)))
    };
    progress->Call(1, argv);
  }

 private:
  Callback *progress;
  int milliseconds;
  int iters;
};

NAN_METHOD(DoProgress) {

    v8::Local<v8::Function> _progress;
    v8::Local<v8::Function> _callback;

    uint32_t i, j;
    std::string error;

    if (Nan::Check(info).ArgumentsCount(4)
        .Argument(0).Bind(i)
        .Argument(1).Bind(j)
        .Argument(2).IsFunction().Bind(_progress)
        .Argument(3).IsFunction().Bind(_callback).Error(&error))
    {
        Callback *progress = new Callback(_progress);
        Callback *callback = new Callback(_callback);
        AsyncQueueWorker(new ProgressWorker(callback, progress, i, j));
    }
    else
    {
        ThrowSyntaxError(error.c_str());
    }
}

NAN_MODULE_INIT(Init) {
  Set(target
    , New<v8::String>("a").ToLocalChecked()
    , New<v8::FunctionTemplate>(DoProgress)->GetFunction());
}

NODE_MODULE(asyncprogressworker, Init)
