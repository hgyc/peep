#include "process.hpp"

#include <tlhelp32.h>
#include <cstring>

DWORD find_pid(const char* name) {
    Handle snap(CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0));
    if (!snap.valid()) return 0;

    PROCESSENTRY32 pe{};
    pe.dwSize = sizeof(pe);

    if (!Process32First(snap.get(), &pe)) return 0;

    do {
        if (_stricmp(pe.szExeFile, name) == 0)
            return pe.th32ProcessID;
    } while (Process32Next(snap.get(), &pe));

    return 0;
}

Handle open_process(DWORD pid) {
    return Handle(OpenProcess(
        PROCESS_QUERY_INFORMATION | PROCESS_VM_READ,
        FALSE,
        pid));
}