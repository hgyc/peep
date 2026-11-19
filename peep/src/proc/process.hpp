#pragma once

#include <windows.h>
#include "../base/handle.hpp"

DWORD find_pid(const char* name);
Handle open_process(DWORD pid);