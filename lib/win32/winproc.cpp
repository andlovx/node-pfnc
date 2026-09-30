// --------------------------------------------------------
// Program path using network connection port (PFNC)
// License: MIT
//
// Copyright (C) 2024 Anders Lövgren, Xertified AB.
// --------------------------------------------------------

#include <windows.h>
#include <tlhelp32.h>
#include <shlwapi.h>
#include <iostream>
#include <vector>

#include "process.hpp"
#include "error.hpp"

#pragma warning(disable : 4244)

//
// Always use the wide character (W) version of the Windows API so that paths
// containing non-ASCII characters (i.e. Chinese or Hindi) are preserved. The
// path is converted to UTF-8 when returned from Process::get_filepath().
//

class ProcessReader
{
public:
    ProcessReader();

    void set_filename(int pid);
    const std::wstring &get_filename() const;

private:
    bool set_filename1(int pid);
    bool set_filename2(int pid);

    std::wstring filename;
};

ProcessReader::ProcessReader()
{
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

    HANDLE processHandle = OpenProcess(dwDesiredAccess, FALSE, pid);
    if (!processHandle)
    {
        error.suppress();
        error.write("Failed open process", pid);
        error.restore();
        return false;
    }

    //
    // Use buffer large enough for extended-length paths (not limited to MAX_PATH).
    //
    std::vector<wchar_t> buffer(32768);
    DWORD size = static_cast<DWORD>(buffer.size());

    if (!QueryFullProcessImageNameW(processHandle, 0, buffer.data(), &size))
    {
        error.write("Failed to get process image name");
        CloseHandle(processHandle);
        return false;
    }

    CloseHandle(processHandle);
    filename.assign(buffer.data(), size);
    return true;
}

bool ProcessReader::set_filename2(int pid)
{
    ErrorLogger error(false);
    PROCESSENTRY32W pe32;
    pe32.dwSize = sizeof(pe32);

    HANDLE snapshotHandle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snapshotHandle == INVALID_HANDLE_VALUE)
    {
        error.write("Could not open process snapshot");
        return false;
    }

    BOOL bResult = Process32FirstW(snapshotHandle, &pe32);
    if (!bResult)
    {
        error.write("Could not open first snapshot");
    }

    while (bResult)
    {
        if (pe32.th32ProcessID == static_cast<DWORD>(pid))
        {
            //
            // The filename's returned is just the filename without any path. Use
            // PathFindOnPath() to search for the binary in standard location such as
            // the system map and all directories in the PATH.
            //
            PathFindOnPathW(pe32.szExeFile, 0);
            filename.assign(pe32.szExeFile);
            break;
        }
        bResult = Process32NextW(snapshotHandle, &pe32);
    }

    CloseHandle(snapshotHandle);
    return true;
}

const std::wstring &ProcessReader::get_filename() const
{
    return filename;
}

void Process::set_path(int pid)
{
    ProcessReader reader;

    reader.set_filename(pid);
    path.assign(reader.get_filename());
}
