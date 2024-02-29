// --------------------------------------------------------
// Program path using network connection port (PFNC)
// License: MIT
//
// Copyright (C) 2024 Anders Lövgren, Xertified AB.
// --------------------------------------------------------

#include <iostream>
#include "error.hpp"

namespace
{
    bool isVerbose()
    {
        return getenv("PFNC_VERBOSE") != nullptr && std::string(getenv("PFNC_VERBOSE")) == "1";
    }
}

std::ostream &operator<<(std::ostream &out, const SystemError &err)
{
    return out << err.message << " (" << err.code << ")";
}

ErrorLogger::ErrorLogger() : _silent(true)
{
}

ErrorLogger::ErrorLogger(bool silent) : _silent(silent)
{
}

void ErrorLogger::silent(bool enable)
{
    _silent = enable;
}

void ErrorLogger::write(const char *reason)
{
    if (!_silent)
    {
        std::cerr << reason << ": " << SystemError() << std::endl;
    }
}

void ErrorLogger::write(const char *reason, int pid)
{
    if (!_silent)
    {
        std::cerr << reason << " (" << pid << ")"
                  << ": " << SystemError() << std::endl;
    }
}

void ErrorLogger::suppress()
{
    _ssaved = _silent;
    _silent = !isVerbose();
}

void ErrorLogger::restore()
{
    _silent = _ssaved;
}
