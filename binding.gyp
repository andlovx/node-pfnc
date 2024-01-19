{
  "targets": [
    {
      "target_name": "pfnc",
      "sources": [ "index.cpp", "lib/error.cpp", "lib/options.cpp", "lib/process.cpp", "lib/scanner.cpp" ],
      'include_dirs': [
        'lib',
      ],
      'conditions': [
        ['OS=="win"', {
            "sources": ["lib/win32/netstat.cpp", "lib/win32/syserr.cpp", "lib/win32/winproc.cpp"]
        }],
        ['OS=="linux"', {
            "sources": ["lib/linux/netstat.cpp", "lib/linux/procfs.cpp", "lib/linux/syserr.cpp"]
        }],
        ['OS=="mac"', {
            "sources": ["lib/macos/netstat.cpp", "lib/macos/syserr.cpp", "lib/macos/sysproc.cpp"]
        }],
      ],
    }
  ]
}
