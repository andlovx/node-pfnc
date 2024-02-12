// --------------------------------------------------------
// Program path using network connection port (PFNC)
// License: MIT
//
// Copyright (C) 2024 Anders Lövgren, Xertified AB.
// --------------------------------------------------------

#include <unistd.h>
#include <limits.h>
#include <sstream>

#include "process.hpp"
#include "error.hpp"

void Process::set_path(int pid)
{
    ErrorLogger error(false);
    std::stringstream link;
    link << "/proc/" << pid << "/exe";

    char buff[PATH_MAX];
    int bytes = 0;

    if ((bytes = readlink(link.str().c_str(), buff, sizeof(buff))) == -1)
    {
        error.write("readlink", pid);
        return;
    }

    buff[bytes] = '\0';
    path.assign(buff);
}
