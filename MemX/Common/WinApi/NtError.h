#pragma once

#include "WinHeaders.h"

//NTSTATUS offset in TEB structure 
//for different architectures
#ifdef _WIN64
#define NT_STATUS_OFFSET 0x1250
#else
#define NT_STATUS_OFFSET 0x00B0
#endif

inline NTSTATUS GetLastNtStatus() {
	return *(NTSTATUS*) ((BYTE*) NtCurrentTeb() + NT_STATUS_OFFSET);
}

inline VOID SetLastNtStatus(NTSTATUS status) {
	*(NTSTATUS*) ((BYTE*) NtCurrentTeb() + NT_STATUS_OFFSET) = status;
}
