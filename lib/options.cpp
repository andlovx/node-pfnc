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

    output.addr = false;
    output.origin = true;
    output.path = true;
    output.pid = true;
    output.port = true;
    output.proto = false;

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

std::ostream &operator<<(std::ostream &out, const Options::Output &output)
{
    if (output.addr)
    {
        out << "Address ";
    }
    if (output.origin)
    {
        out << "Origin ";
    }
    if (output.path)
    {
        out << "Path ";
    }
    if (output.pid)
    {
        out << "PID ";
    }
    if (output.port)
    {
        out << "Port ";
    }
    if (output.proto)
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
