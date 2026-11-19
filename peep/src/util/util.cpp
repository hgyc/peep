#include "util.hpp"
#include <cstdio>

std::string prot_str(DWORD protect) {
    DWORD base = protect & 0xFF;
    std::string s;

    switch (base) {
    case PAGE_NOACCESS:          s = "NA";   break;
    case PAGE_READONLY:          s = "R";    break;
    case PAGE_READWRITE:         s = "RW";   break;
    case PAGE_WRITECOPY:         s = "WC";   break;
    case PAGE_EXECUTE:           s = "X";    break;
    case PAGE_EXECUTE_READ:      s = "RX";   break;
    case PAGE_EXECUTE_READWRITE: s = "RWX";  break;
    case PAGE_EXECUTE_WRITECOPY: s = "XWC";  break;
    default:                     s = "?";    break;
    }

    if (protect & PAGE_GUARD)         s += "+G";
    if (protect & PAGE_NOCACHE)       s += "+NC";
    if (protect & PAGE_WRITECOMBINE)  s += "+WCM";

    return s;
}

std::string type_str(DWORD type) {
    switch (type) {
    case MEM_IMAGE:   return "IMAGE";
    case MEM_MAPPED:  return "MAPPED";
    case MEM_PRIVATE: return "PRIVATE";
    default:          return "UNKNOWN";
    }
}

std::string win_error(DWORD code) {
    char* buf = nullptr;
    DWORD len = FormatMessageA(
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        nullptr, code, 0,
        reinterpret_cast<char*>(&buf), 0, nullptr);

    std::string result = len ? buf : "unknown error";
    if (buf) LocalFree(buf);
    return result;
}

std::string to_hex(uintptr_t value) {
    char buf[32];
    std::snprintf(buf, sizeof(buf), "%016llX",
        static_cast<unsigned long long>(value));
    return buf;
}