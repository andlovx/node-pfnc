// --------------------------------------------------------
// Program path using network connection port (PFNC)
// License: MIT
//
// Copyright (C) 2024 Anders Lövgren, Xertified AB.
// --------------------------------------------------------

#ifndef SCANNER_HPP
#define SCANNER_HPP

#include <functional>

#include "options.hpp"
#include "netstat.hpp"

class Scanner
{
    using Entry = Netstat::Entry;

public:
    Scanner(const Options &options);
    void start(const std::function<void(const Entry &)>) const;

private:
    const Options &_options;
};

#endif // SCANNER_HPP
