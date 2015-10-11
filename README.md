Arguments checking in native modules for Node.js
===============================

[![Build Status](https://travis-ci.org/BloodAxe/nan-check.png?branch=master)](https://travis-ci.org/BloodAxe/nan-check)

This project contains some helper utilities that make native addon development a bit more easier.

 * **[Example](#example)**
 * **[Usage](#usage)**
 * **[API](#api)**
 * **[Tests](#tests)**

<a name="example"></a>
## Example

```cpp
NAN_METHOD(calibrationPatternDetect)
{
    NanScope();

    Local<Object>   imageBuffer;
    Local<Function> callback;
    cv::Size        patternSize;
    PatternType     pattern;

    try
    {
        if (Nan::Check(info).ArgumentsCount(5)
            .Argument(0).IsBuffer().Bind(imageBuffer)
            .Argument(1).Bind(patternSize.width)
            .Argument(2).Bind(patternSize.height)
            .Argument(3).StringEnum<PatternType>({ 
                { "CHESSBOARD",     PatternType::CHESSBOARD }, 
                { "CIRCLES_GRID",   PatternType::CIRCLES_GRID }, 
                { "ACIRCLES_GRID",  PatternType::ACIRCLES_GRID } }).Bind(pattern)
            .Argument(4).IsFunction().Bind(callback))
        {
            Nan::Callback *nanCallback = new Nan::Callback(callback);
            Nan::AsyncQueueWorker(new DetectPatternTask(imageBuffer, patternSize, pattern, nanCallback));
            Nan::ReturnValue(Nan::True());
        }

        Nan::ReturnValue(Nan::False());
    }
    catch (Nan::CheckException& exc)
    {
        return Nan::ThrowTypeError(exc.what());
    }
}
```

<a name="usage"></a>
## Usage

Simply add **NAN-Check** as a dependency in the *package.json* of your Node addon:

``` bash
$ npm install --save nan nan-check
```
Pull in the path to **NAN** and **NAN-Check** in your *binding.gyp* so that you can use `#include <nan-check.h>` in your *.cpp* files:

``` python
"include_dirs" : [
    "<!(node -e \"require('nan')\")",
    "<!(node -e \"require('nan-check')\")"
]
```

This works like a `-I<path-to-NAN-Check>` when compiling your addon.

<a name="api"></a>
## API

TODO: Write

<a name="tests"></a>
### Tests

To run the NAN-Check tests do:

``` sh
npm install
npm run-script rebuild-tests
npm test
```

Or just:

``` sh
npm install
make test
```

## Licence &amp; copyright

Copyright (c) 2015 Ievgen Khvedchenia.

Native Abstractions for Node.js is licensed under an MIT license. 
All rights not explicitly granted in the MIT license are reserved. 
See the included LICENSE file for more details.
