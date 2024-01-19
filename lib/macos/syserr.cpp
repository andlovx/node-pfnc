// --------------------------------------------------------
// Program path using network connection port (PFNC)
// License: MIT
//
// Copyright (C) 2024 Anders Lövgren, Xertified AB.
// --------------------------------------------------------

#include <cstring>
#include <cerrno>

#include "error.hpp"

SystemError::SystemError() : SystemError(errno)
{
}

SystemError::SystemError(ErrorCode code) : code(code)
{
    message = strerror(code);
}

SystemError::~SystemError()
{
}
