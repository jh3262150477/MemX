#pragma once

#include "../Common/WinApi/WinHeaders.h"
#include "../Common/Types/Type.h"
#include "../Config.h"

namespace MemX {
	class Adapter {
		public:
		MEMX_API Adapter(HANDLE hProcess);
		MEMX_API ~Adapter();

		NTSTATUS ReadProcessMemoryT(ptr_t lpBaseAddress, LPVOID lpBuffer, size_t dwSize, SIZE_T* readBytes);

		NTSTATUS WriteProcessMemoryT(ptr_t lpBaseAddress, LPCVOID lpBuffer, size_t dwSize, SIZE_T* writtenBytes = NULL);

		NTSTATUS VirtualQueryExT(ptr_t lpAddress, PMEMORY_BASIC_INFORMATION64 lpBuffer);

		private:
		HANDLE _hProcess;
	};
}

