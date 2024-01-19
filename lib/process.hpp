// --------------------------------------------------------
// Program path using network connection port (PFNC)
// License: MIT
//
// Copyright (C) 2024 Anders Lövgren, Xertified AB.
// --------------------------------------------------------

#ifndef PROCESS_HPP
#define PROCESS_HPP

#include <string>
#include <filesystem>

class Process
{
public:
    Process(int pid);

    const std::filesystem::path &get_path() const;
    std::string get_filename() const;
    std::string get_filepath() const;

    operator std::string() { return get_filepath(); }

private:
    void set_path(int pid);
    std::filesystem::path path;
};

#endif // PROCESS_HPP
