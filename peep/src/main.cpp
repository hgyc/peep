#include <windows.h>
#include <cstdio>
#include <cstdlib>

#include "base/handle.hpp"
#include "util/util.hpp"
#include "proc/process.hpp"
#include "mem/memory.hpp"

int main(int argc, char** argv) {
    if (argc != 2) {
        printf("usage: peep.exe <pid|name>\n");
        return 1;
    }

    DWORD pid = 0;
    char* end = nullptr;
    unsigned long v = std::strtoul(argv[1], &end, 10);

    if (end != argv[1] && *end == '\0') {
        pid = static_cast<DWORD>(v);         

    }
    else {
        pid = find_pid(argv[1]);             
        if (pid == 0) {
            printf("[!] process not found: %s\n", argv[1]);
            return 2;
        }
    }

    Handle proc = open_process(pid);
    if (!proc.valid()) {
        printf("[!] OpenProcess failed: %s\n",
            win_error(GetLastError()).c_str());
        return 3;
    }
    else {
        printf("[*] PID %lu opened\n", pid);
        walk_memory(proc.get());
        return 0;
    }

    printf("[*] PID %lu opened\n", pid);
    return 0;
}