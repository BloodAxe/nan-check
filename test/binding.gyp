{
  "target_defaults":
  {
      "cflags" : ["-Wall", "-Wextra", "-Wno-unused-parameter"],
      "include_dirs":
      [
        "<!(node -e \"require('..')\")",
        "<!(node -e \"require('nan')\")",
        "<!(node -e \"require('nan-marshal')\")",
      ]
  },
  "targets": [
  {
      "target_name" : "asyncprogressworker",
      "sources"     : [
        "cpp/asyncprogressworker.cpp"
        ],
      'target_conditions': [
        ['OS=="mac"', {
            'xcode_settings': {
                'GCC_ENABLE_CPP_EXCEPTIONS': 'YES',
                'GCC_ENABLE_CPP_RTTI': 'YES',
                'OTHER_CFLAGS': [ '-g', '-mmacosx-version-min=10.7', '-std=c++11', '-stdlib=libc++' ],
                'OTHER_CPLUSPLUSFLAGS': [ '-g', '-mmacosx-version-min=10.7', '-std=c++11', '-stdlib=libc++' ]
            }
        }],
        ['OS=="linux" or OS=="freebsd" or OS=="openbsd" or OS=="solaris"', {

            'libraries!': [ '-undefined dynamic_lookup' ],
            'cflags_cc!': [ '-fno-exceptions', '-fno-rtti' ],
            "cflags": [ '-std=c++11', '-fexceptions', '-frtti' ],
        }]
      ]
  },
  {
    "target_name" : "function_with_numeric_parameter",
      "sources"     : [
        "cpp/function_with_numeric_parameter.cpp"
        ],
      'target_conditions': [
        ['OS=="mac"', {
            'xcode_settings': {
                'GCC_ENABLE_CPP_EXCEPTIONS': 'YES',
                'GCC_ENABLE_CPP_RTTI': 'YES',
                'OTHER_CFLAGS': [ '-g', '-mmacosx-version-min=10.7', '-std=c++11', '-stdlib=libc++' ],
                'OTHER_CPLUSPLUSFLAGS': [ '-g', '-mmacosx-version-min=10.7', '-std=c++11', '-stdlib=libc++' ]
            }
        }],
        ['OS=="linux" or OS=="freebsd" or OS=="openbsd" or OS=="solaris"', {

            'libraries!': [ '-undefined dynamic_lookup' ],
            'cflags_cc!': [ '-fno-exceptions', '-fno-rtti' ],
            "cflags": [ '-std=c++11', '-fexceptions', '-frtti' ],
        }]
      ]
  }

]}
