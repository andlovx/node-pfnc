// --------------------------------------------------------
// Program path using network connection port (PFNC)
// License: MIT
//
// Copyright (C) 2024 Anders Lövgren, Xertified AB.
// --------------------------------------------------------

#ifndef NETSTAT_HPP
#define NETSTAT_HPP

#include <iostream>
#include <string>
#include <functional>
#include <cstdint>

class Netstat
{
public:
    struct Address
    {
        std::string addr;
        uint16_t port;
    };

    struct Entry
    {
        Address local;
        Address foreign;
        int pid;
        std::string program;
        std::string path;
        std::string proto;
    };

    void discover(const std::function<void(const Entry &)>) const;
};

inline std::ostream &operator<<(std::ostream &out, const Netstat::Address &address)
{
    return out << '{'
               << "addr: " << address.addr << ", "
               << "port: " << address.port
               << '}';
}

inline std::ostream &operator<<(std::ostream &out, const Netstat::Entry &entry)
{
    return out << '{'
               << "proto: " << entry.proto << ", "
               << "pid: " << entry.pid << ", "
               << "local: " << entry.local << ", "
               << "foreign: " << entry.foreign << ", "
               << "program: " << entry.program << ", "
               << "path: " << entry.path
               << '}';
}

#endif // NETSTAT_HPP
