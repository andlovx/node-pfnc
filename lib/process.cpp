// --------------------------------------------------------
// Program path using network connection port (PFNC)
// License: MIT
//
// Copyright (C) 2024 Anders Lövgren, Xertified AB.
// --------------------------------------------------------

#include "process.hpp"
#include "sysdep.hpp"

#ifdef PLATFORM_WINDOWS
#include <windows.h>
#endif

namespace
{
    //
    // Convert path to UTF-8 encoded string. On Windows is the native path encoding
    // UTF-16 and path::string() would convert to the ANSI code page, garbling any
    // characters not representable in it (or throw).
    //
    std::string to_utf8(const std::filesystem::path &path)
    {
#ifdef PLATFORM_WINDOWS
        const std::wstring &wide = path.native();
        if (wide.empty())
        {
            return std::string();
        }

        int size = WideCharToMultiByte(CP_UTF8, 0, wide.data(), static_cast<int>(wide.size()), nullptr, 0, nullptr, nullptr);
        if (size <= 0)
        {
            return std::string();
        }

        std::string result(size, '\0');
        WideCharToMultiByte(CP_UTF8, 0, wide.data(), static_cast<int>(wide.size()), result.data(), size, nullptr, nullptr);
        return result;
#else
        return path.string();
#endif
    }
}

Process::Process(int pid)
{
    set_path(pid);
}

const std::filesystem::path &Process::get_path() const
{
    return path;
}

std::string Process::get_filename() const
{
    return to_utf8(path.filename());
}

std::string Process::get_filepath() const
{
    return to_utf8(path);
}
