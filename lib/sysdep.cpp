// --------------------------------------------------------
// Program path using network connection port (PFNC)
// License: MIT
//
// Copyright (C) 2024 Anders Lövgren, Xertified AB.
// --------------------------------------------------------

#include "sysdep.hpp"

#ifdef PLATFORM_WINDOWS
#include "win32/netstat.cpp"
#include "win32/syserr.cpp"
#include "win32/winproc.cpp"
#endif

#ifdef PLATFORM_DARWIN
#include "macos/netstat.cpp"
#include "macos/syserr.cpp"
#include "macos/sysproc.cpp"
#endif

#ifdef PLATFORM_LINUX
#include "linux/netstat.cpp"
#include "linux/syserr.cpp"
#include "linux/procfs.cpp"
#endif
