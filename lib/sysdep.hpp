// --------------------------------------------------------
// Program path using network connection port (PFNC)
// License: MIT
//
// Copyright (C) 2024 Anders Lövgren, Xertified AB.
// --------------------------------------------------------

#ifndef SYSDEP_HPP
#define SYSDEP_HPP

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#define PLATFORM_NAME "windows"
#define PLATFORM_WINDOWS 1
#endif

#if defined(__linux__)
#define PLATFORM_NAME "linux"
#define PLATFORM_LINUX 1
#endif

#if defined(__APPLE__)
#define PLATFORM_NAME "darwin"
#define PLATFORM_MACOS 1
#endif

#if defined(PLATFORM_LINUX) || defined(PLATFORM_MACOS)
#define PLATFORM_UNIX 1
#endif

#if defined(PLATFORM_MACOS)
#define PLATFORM_DARWIN 1
#endif

#endif // SYSDEP_HPP
