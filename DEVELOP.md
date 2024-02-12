# Developer notes

Building this extension requires a C++ compiler with support for the C++-17 standard.

## Building

Build extension using node-gyp and installed C++ compiler:

```shell
npm run install
```

## Testing

Start a netcat server and client (in different terminals).

```shell
nc -v -l localhost 4746     # server
nc -v -d localhost 4746     # client
```

The file `driver.js` can be used for testing from command line.

```shell
node driver.js
```

## Debugging

Install segfault handler for generating stack traces if the native code crashes.

```shell
npm install segfault-handler
```

Register a handler in the test driver `driver.js`:

```javascript
const SegfaultHandler = require("segfault-handler");
SegfaultHandler.registerHandler("crash.log");
```

Another option is to build the extension with debugging symbols and then run node under i.e. gdb.
