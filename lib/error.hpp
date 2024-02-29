// --------------------------------------------------------
// Program path using network connection port (PFNC)
// License: MIT
//
// Copyright (C) 2024 Anders Lövgren, Xertified AB.
// --------------------------------------------------------

#ifndef ERROR_HPP
#define ERROR_HPP

#include "sysdep.hpp"

#if PLATFORM_WINDOWS
#include <windows.h>
typedef DWORD ErrorCode;
typedef LPTSTR ErrorString;
#else
typedef int ErrorCode;
typedef const char *ErrorString;
#endif

class ErrorLogger
{
public:
    ErrorLogger();
    ErrorLogger(bool silent);

    void silent(bool enable);
    void write(const char *reason);
    void write(const char *reason, int pid);

    void suppress();
    void restore();

private:
    bool _silent;
    bool _ssaved;
};

struct SystemError
{
    SystemError();
    SystemError(ErrorCode code);
    ~SystemError();

    ErrorCode code;
    ErrorString message;
};

#endif // ERROR_HPP
