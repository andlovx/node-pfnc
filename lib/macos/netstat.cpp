// --------------------------------------------------------
// Program path using network connection port (PFNC)
// License: MIT
//
// Copyright (C) 2024 Anders Lövgren, Xertified AB.
// --------------------------------------------------------

#include <regex>
#include <iostream>
#include <cstdio>
#include <array>

#include "netstat.hpp"
#include "process.hpp"

namespace
{
    class Runner
    {
    public:
        Runner() : handle(popen("netstat -anv", "r")) {}
        ~Runner() { pclose(handle); }

        operator FILE *() { return handle; }

    private:
        FILE *handle;
    };

    class Buffer : public std::array<char, 128>
    {
    public:
        Buffer();
        bool populate(Netstat::Entry &);

    private:
        std::regex regexp;
    };
}

void Netstat::discover(const std::function<void(const Entry &)> callback) const
{
    Runner process;
    Buffer buffer;
    Entry entry;

    while (std::fgets(buffer.data(), buffer.size(), process))
    {
        if (buffer.populate(entry))
        {
            callback(entry);
        }
    }
}

Buffer::Buffer()
{
    regexp = std::regex("(tcp[4,6]|udp[4,6]).*?([\\.0-9]+|[0-9a-f:%]+|\\*)\\.(\\d+|\\*)\\s+([\\.0-9]+|[0-9a-f:%]+|\\*)\\.(\\d+|\\*)\\s+(\\w+\\s+){3}(\\d+)\\s+(\\d+)");
}

bool Buffer::populate(Netstat::Entry &entry)
{
    std::smatch matches;
    std::string subject(data());

    if (!std::regex_search(subject, matches, regexp))
    {
        return false;
    }
    if (matches.size() != 9)
    {
        std::cerr << "Expected 9 matches, got " << matches.size() << std::endl;
        return false;
    }

    entry.proto = matches[1];
    entry.local.addr = matches[2];
    entry.local.port = atoi(matches[3].str().c_str());
    entry.foreign.addr = matches[4];
    entry.foreign.port = atoi(matches[5].str().c_str());
    entry.pid = atoi(matches[7].str().c_str());

    if (entry.pid)
    {
        Process process(entry.pid);
        entry.path = process.get_filepath();
    }

    return true;
}
