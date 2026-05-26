// --------------------------------------------------------
// Program path using network connection port (PFNC)
// License: MIT
//
// Copyright (C) 2024 Anders Lövgren, Xertified AB.
// --------------------------------------------------------

#include <windows.h>
#include <tchar.h>
#include <tlhelp32.h>
#include <shlwapi.h>
#include <iostream>

#include "process.hpp"
#include "error.hpp"

#pragma warning(disable : 4244)

class ProcessReader
{
public:
    ProcessReader();
    ~ProcessReader();

    void set_filename(int pid);
    std::string get_filename() const;

private:
    bool set_filename1(int pid);
    bool set_filename2(int pid);

    HANDLE processHandle;
    TCHAR filename[MAX_PATH];
};

ProcessReader::ProcessReader() : processHandle(nullptr)
{
    memset(filename, 0, sizeof(filename));
}

ProcessReader::~ProcessReader()
{
    if (processHandle)
    {
        CloseHandle(processHandle);
    }
}

void ProcessReader::set_filename(int pid)
{
    if (set_filename1(pid))
    {
        return;
    }
    set_filename2(pid);
}

bool ProcessReader::set_filename1(int pid)
{
    ErrorLogger error(false);
    DWORD dwDesiredAccess = PROCESS_QUERY_LIMITED_INFORMATION;

    processHandle = OpenProcess(dwDesiredAccess, FALSE, pid);
    if (!processHandle)
    {
        error.suppress();
        error.write("Failed open process", pid);
        error.restore();
        return false;
    }

    DWORD size = MAX_PATH;
    if (!QueryFullProcessImageName(processHandle, 0, filename, &size))
    {
        error.write("Failed to get process image name");
        return false;
    }

    return true;
}

bool ProcessReader::set_filename2(int pid)
{
    ErrorLogger error(false);
    PROCESSENTRY32 pe32;
    pe32.dwSize = sizeof(pe32);

    processHandle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (processHandle == INVALID_HANDLE_VALUE)
    {
        error.write("Could not open process snapshot");
        return false;
    }

    BOOL bResult = Process32First(processHandle, &pe32);
    if (!bResult)
    {
        error.write("Could not open first snapshot");
    }

    while (bResult)
    {
        if (pe32.th32ProcessID == pid)
        {
            //
            // The filename's returned is just the filename without any path. Use
            // PathFindOnPath() to search for the binary in standard location such as
            // the system map and all directories in the PATH.
            //
            PathFindOnPath(pe32.szExeFile, 0);
            memcpy(filename, pe32.szExeFile, MAX_PATH);
            break;
        }
        bResult = Process32Next(processHandle, &pe32);
    }

    return true;
}

std::string ProcessReader::get_filename() const
{
    std::string result;

#ifndef UNICODE
    result = filename;
#else
    std::wstring ff(filename);
    result = std::string(ff.begin(), ff.end());
#endif

    return result;
}

void Process::set_path(int pid)
{
    ProcessReader reader;

    reader.set_filename(pid);
    path.assign(reader.get_filename());
}
