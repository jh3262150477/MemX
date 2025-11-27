#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif // Win32 lean and mean

#include <windows.h>
#include <tlhelp32.h>
#include <winternl.h>

#pragma warning(push)
#pragma warning(disable: 4005) 
#include <ntstatus.h>
#pragma warning(pop)
