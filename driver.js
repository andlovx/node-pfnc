//
// Test driver for development.
//

const pfnc = require("bindings")("pfnc");

console.log(
  "LOCAL",
  pfnc.scan({
    port: 4746,
    origin: "local",
  })
);

console.log(
  "REMOTE",
  pfnc.scan({
    port: 4746,
    origin: "remote",
  })
);

console.log(
  "BOTH",
  pfnc.scan({
    port: 4746,
    origin: "both",
  })
);
