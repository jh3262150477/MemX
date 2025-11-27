#include "Adapter.h"
#include "../Common/WinApi/NtError.h"


namespace MemX {
	Adapter::Adapter(HANDLE hProcess) : _hProcess(hProcess) {
		
	}

	Adapter::~Adapter() {
	}


	NTSTATUS Adapter::ReadProcessMemoryT(ptr_t lpBaseAddress, LPVOID lpBuffer, size_t dwSize, SIZE_T* readBytes) {
		DWORD dwRet = ReadProcessMemory(_hProcess, reinterpret_cast<LPCVOID>(lpBaseAddress), lpBuffer, dwSize, readBytes);
		return dwRet != 0 ? STATUS_SUCCESS : GetLastNtStatus();
	}

	NTSTATUS Adapter::WriteProcessMemoryT(ptr_t lpBaseAddress, LPCVOID lpBuffer, size_t dwSize, SIZE_T* writtenBytes) {
		DWORD dwRet = WriteProcessMemory(_hProcess, reinterpret_cast<LPVOID>(lpBaseAddress), lpBuffer, dwSize, writtenBytes);
		return dwRet != 0 ? STATUS_SUCCESS : GetLastNtStatus();
	}


	NTSTATUS Adapter::VirtualQueryExT(ptr_t lpAddress, PMEMORY_BASIC_INFORMATION64 lpBuffer) {
		DWORD64 dwRet = VirtualQueryEx(_hProcess, reinterpret_cast<LPCVOID>(lpAddress), (PMEMORY_BASIC_INFORMATION) lpBuffer, sizeof(MEMORY_BASIC_INFORMATION64));
		return dwRet != 0 ? STATUS_SUCCESS: GetLastNtStatus();
	}
}