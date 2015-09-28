{
  "target_defaults":
    {
        "cflags" : ["-Wall", "-Wextra", "-Wno-unused-parameter"],
        "include_dirs": ["<!(node -e \"require('..')\")", "<!(node -e \"require('nan')\")",]
    },
  "targets": [
    {
        "target_name" : "asyncprogressworker"
      , "sources"     : [ "cpp/asyncprogressworker.cpp" ]
    }
]}
