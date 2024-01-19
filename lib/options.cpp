// --------------------------------------------------------
// Program path using network connection port (PFNC)
// License: MIT
//
// Copyright (C) 2024 Anders Lövgren, Xertified AB.
// --------------------------------------------------------

#include <sstream>
#include <exception>

#include "options.hpp"

Options::Options()
{
    source.origin = PortOrigin::Unspec;
    source.port = 0;

    output.format = OutputFormat::Table;
    output.list.addr = false;
    output.list.origin = true;
    output.list.path = true;
    output.list.pid = true;
    output.list.port = true;
    output.list.proto = false;

    runtime.debug = false;
    runtime.verbose = false;
}

std::ostream &operator<<(std::ostream &out, const PortOrigin &origin)
{
    switch (origin)
    {
    case PortOrigin::Unspec:
        return out << "Unspec";
    case PortOrigin::Local:
        return out << "Local";
    case PortOrigin::Remote:
        return out << "Remote";
    case PortOrigin::Either:
        return out << "Either";
    default:
        return out << "";
    }
}

std::ostream &operator<<(std::ostream &out, const Options::Output::List &list)
{
    if (list.addr)
    {
        out << "Address ";
    }
    if (list.origin)
    {
        out << "Origin ";
    }
    if (list.path)
    {
        out << "Path ";
    }
    if (list.pid)
    {
        out << "PID ";
    }
    if (list.port)
    {
        out << "Port ";
    }
    if (list.proto)
    {
        out << "Proto ";
    }

    return out;
}

std::ostream &operator<<(std::ostream &out, const Options &options)
{
    return out
           << "Source:\n"
           << "   Origin: " << options.source.origin << "\n"
           << "     Port: " << options.source.port << "\n"
           << "Runtime:\n"
           << "    Debug: " << options.runtime.debug << "\n"
           << "  Verbose: " << options.runtime.verbose << "\n";
}
