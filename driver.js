//
// Test driver for development.
//

const pfnc = require("bindings")("pfnc");

const options = {
  port: 4746,
  origin: "both",
};

console.log(pfnc.scan(options));
