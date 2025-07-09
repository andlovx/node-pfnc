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

    class Buffer : public std::array<char, 256>
    {
    public:
        bool populate(Netstat::Entry &);
        void setPattern(const std::string &header);

    private:
        std::regex regexp;
    };

    class Header 
    {
        public:
            enum class Format
            {
                Unknown,
                MacOS14,
                MacOS15
            };

            Header(const Buffer &buffer);
            Header(const std::string &header);
            Header(FILE *stream);

            Format getFormat() const;
            Format getFormat(const std::string &header) const;

        private:
            void setFormat(const std::string &header);
            bool isFormat14(const std::string &header) const;
            bool isFormat15(const std::string &header) const;
            bool isMatched(const std::string &header, const std::regex &regex, std::smatch matches = std::smatch()) const;

            Format format;
    };

    Header::Header(const Buffer &buffer)
    {
        setFormat(buffer.data());
    }

    Header::Header(const std::string &header)
    {
        setFormat(header);
    }

    Header::Header(FILE *stream)
    {
        Buffer buffer;

        std::fgets(buffer.data(), buffer.size(), stream);
        std::fgets(buffer.data(), buffer.size(), stream);

        setFormat(buffer.data());
    }

    Header::Format Header::getFormat() const
    {
        return format;
    }

    Header::Format Header::getFormat(const std::string &header) const
    {
        if (isFormat14(header)) {
            return Format::MacOS14;
        } else if (isFormat15(header)) {
            return Format::MacOS15;
        } else {
            return Format::Unknown;
        }
    }

    void Header::setFormat(const std::string &header)
    {
        format = getFormat(header);
    }

    bool Header::isFormat14(const std::string &header) const
    {
        return isMatched(header, std::regex("Proto\\s+Recv-Q\\s+Send-Q\\s+Local Address\\s+Foreign Address\\s+\\(state\\)\\s+rhiwat\\s+shiwat\\s+(pid)\\s+epid\\s+state\\s+options\\s+gencnt\\s+flags\\s+flags1\\s+usscnt\\s+rtncnt\\s+fltrs\n"));
    }

    bool Header::isFormat15(const std::string &header) const 
    {
        return isMatched(header, std::regex("Proto\\s+Recv-Q\\s+Send-Q\\s+Local Address\\s+Foreign Address\\s+\\(state\\)\\s+rxbytes\\s+txbytes\\s+rhiwat\\s+shiwat\\s+(pid)\\s+epid\\s+state\\s+options\\s+gencnt\\s+flags\\s+flags1\\s+usecnt\\s+rtncnt\\s+fltrs\n"));
    }

    bool Header::isMatched(const std::string &header, const std::regex &pattern, std::smatch matches) const 
    {
        return std::regex_match(header, matches, pattern);
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
        entry.pid = atoi(matches[8].str().c_str());

        if (entry.pid)
        {
            Process process(entry.pid);
            entry.path = process.get_filepath();
        }

        return true;
    }

    void Buffer::setPattern(const std::string &header) 
    {
        Header match(header);

        switch (match.getFormat())
        {
        case Header::Format::MacOS14:
            regexp = std::regex("(tcp[4,6]|udp[4,6]).*?([\\.0-9]+|[0-9a-f:%]+|\\*)\\.(\\d+|\\*)\\s+([\\.0-9]+|[0-9a-f:%]+|\\*)\\.(\\d+|\\*)\\s+(\\w+)\\s+(\\d+\\s+){2}\\s+(\\d+)");
            break;
        case Header::Format::MacOS15:
            regexp = std::regex("(tcp[4,6]|udp[4,6]).*?([\\.0-9]+|[0-9a-f:%]+|\\*)\\.(\\d+|\\*)\\s+([\\.0-9]+|[0-9a-f:%]+|\\*)\\.(\\d+|\\*)\\s+(\\w+)\\s+(\\d+\\s+){4}\\s+(\\d+)");
            break;
        case Header::Format::Unknown:
        default:
            std::cerr << "Unknown netstat output!" << std::endl;
        }
    }
}

void Netstat::discover(const std::function<void(const Entry &)> callback) const
{
    Runner process;
    Buffer buffer;
    Entry entry;

    std::fgets(buffer.data(), buffer.size(), process);
    std::fgets(buffer.data(), buffer.size(), process);

    buffer.setPattern(buffer.data());

    while (std::fgets(buffer.data(), buffer.size(), process))
    {
        if (buffer.populate(entry))
        {
            callback(entry);
        }
    }
}
