/*********************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2015 NAN contributors
 *
 * MIT License <https://github.com/nodejs/nan/blob/master/LICENSE.md>
 ********************************************************************/

const test = require("tap").test,
  testRoot = require("path").resolve(__dirname, ".."),
  bindings = require("bindings")({
    module_root: testRoot,
    bindings: "function_with_numeric_parameter"
  });

test("isnumber-test", function(t) {
  const errorMessage = "Argument 0 violates IsNumber check";
  const expectedError = new TypeError(errorMessage);
  t.ok(5 === bindings.TestNumericParameter(5), "returns expected value");
  t.throws(
    function() {
      return bindings.TestNumericParameter("foo");
    },
    expectedError,
    "throws TypeError exception on non-numeric parameter"
  );
  t.end();
});
