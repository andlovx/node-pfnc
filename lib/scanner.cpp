// --------------------------------------------------------
// Program path using network connection port (PFNC)
// License: MIT
//
// Copyright (C) 2024 Anders Lövgren, Xertified AB.
// --------------------------------------------------------

#include <iostream>

#include "scanner.hpp"
#include "netstat.hpp"
#include "process.hpp"

Scanner::Scanner(const Options &options)
    : _options(options)
{
}

void Scanner::start(const std::function<void(const Entry &)> callback) const
{
    Netstat netstat;

    netstat.discover([this, &callback](const Netstat::Entry &entry)
                     {
                          if (_options.runtime.debug)
                          {
                             std::cout << entry << std::endl;
                          }

                          if (_options.source.port == entry.local.port && (
                              _options.source.origin == PortOrigin::Local ||
                              _options.source.origin == PortOrigin::Either))
                          {
                              callback(entry);
                          }
                          if (_options.source.port == entry.foreign.port && (
                              _options.source.origin == PortOrigin::Remote ||
                              _options.source.origin == PortOrigin::Either))
                          {
                              callback(entry);
                          } });
}
