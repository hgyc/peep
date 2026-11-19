#include "memory.hpp"

#include "../util/util.hpp"
#include <cstdio>

static bool is_executable(DWORD protect) {
    switch (protect & 0xFF) {
    case PAGE_EXECUTE:
    case PAGE_EXECUTE_READ:
    case PAGE_EXECUTE_READWRITE:
    case PAGE_EXECUTE_WRITECOPY:
        return true;
    default:
        return false;
    }
}

void walk_memory(HANDLE proc) {
    uint8_t* addr = nullptr;
    MEMORY_BASIC_INFORMATION mbi{};

    while (VirtualQueryEx(proc, addr, &mbi, sizeof(mbi))) {
        const DWORD base_prot = mbi.Protect & 0xFF;

        if (mbi.State == MEM_COMMIT &&
            !(mbi.Protect & PAGE_GUARD) &&
            base_prot != PAGE_NOACCESS) {

            printf("%s  size=0x%-10llx  %-5s  %-8s%s\n",
                to_hex(reinterpret_cast<uintptr_t>(mbi.BaseAddress)).c_str(),
                static_cast<unsigned long long>(mbi.RegionSize),
                prot_str(mbi.Protect).c_str(),
                type_str(mbi.Type).c_str(),
                is_executable(mbi.Protect) ? " EXEC" : "");
        }

        uintptr_t next =
            reinterpret_cast<uintptr_t>(mbi.BaseAddress) + mbi.RegionSize;
        if (next <= reinterpret_cast<uintptr_t>(addr))
            break;                              
        addr = reinterpret_cast<uint8_t*>(next);
    }
}