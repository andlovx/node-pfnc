# Program path using network connection (PFNC)

Find application connected against a given UDP/TCP port. The port can refer to either local or remote side of the network connection.

This [Node.JS](https://nodejs.org) extension uses native platform API and commands to query the system for relevant information. Windows, MacOS and Linux are the currently supported operating systems.

## Install

Use NPM to install this extension.

```shell
npm install --save @xertified/pfnc
```

## Usage

Find all applications connected against remote port 443:

```javascript
const pfnc = require("@xertified/pfnc");

const result = pfnc.scan({ port: 443, origin: "local" });
console.log(result);
```

Find all applications connected against local port 443:

```javascript
const pfnc = require("@xertified/pfnc");

const result = pfnc.scan({ port: 443, origin: "remote" });
console.log(result);
```

## Options

Parameter for scan() function is a object containing the port and origin of network connection. The origin member can have either `"local"`, `"remote"` or `"both"` as its value.

## Result

The result is an array of objects, where each object contains at least the `origin`, `port`, `pid` and `path`. This node extension uses platform API calls to resolve the absolute path from the PID associated with the network connection.

### Example

```javascript
[
  {
    origin: "client",
    port: 50022,
    pid: 72569,
    path: "/usr/bin/nc.openbsd",
  },
  {
    origin: "server",
    port: 60304,
    pid: 53628,
    path: "/usr/bin/nc.openbsd",
  },
  {
    origin: "client",
    port: 60304,
    pid: 53631,
    path: "/usr/bin/nc.openbsd",
  },
];
```
