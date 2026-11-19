#pragma once

#include <windows.h>
#include <string>

std::string prot_str(DWORD protect);
std::string type_str(DWORD type);
std::string win_error(DWORD code);
std::string to_hex(uintptr_t value);