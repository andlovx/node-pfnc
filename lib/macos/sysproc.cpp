// --------------------------------------------------------
// Program path using network connection port (PFNC)
// License: MIT
//
// Copyright (C) 2024 Anders Lövgren, Xertified AB.
// --------------------------------------------------------

#include <iostream>
#include <libproc.h>

#include "process.hpp"
#include "error.hpp"

void Process::set_path(int pid)
{
    char buff[PROC_PIDPATHINFO_MAXSIZE];
    proc_pidpath(pid, buff, sizeof(buff));

    path.assign(buff);
}
