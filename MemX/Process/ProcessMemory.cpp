#include "ProcessMemory.h"
#include "Process.h"
#include "../Common/WinApi/ArchitectureApi.h"

namespace MemX {

	ProcessMemory::ProcessMemory(Process* process) : _process(process), _core(process->Core()){
	}

	ProcessMemory::~ProcessMemory() {
	}

	MEMX_API NTSTATUS ProcessMemory::Read(ptr_t baseAddr, PVOID pResult, size_t dwSize, bool skipUncommited) {
		DWORD64 dwRead = 0;
		if ( !baseAddr ) {
			return STATUS_INVALID_ADDRESS;
		}

		if ( !skipUncommited ) {
			NTSTATUS status = _core.getAdapter()->ReadProcessMemoryT(baseAddr, pResult, dwSize, &dwRead);
			if (NT_SUCCESS(status) && dwRead != dwSize) {
				return STATUS_PARTIAL_COPY;
			}
			return status;
		} else {
			MEMORY_BASIC_INFORMATION64 mbi = { 0 };
			ptr_t currentAddr = baseAddr;
			
			while (currentAddr < baseAddr + dwSize) {
				if (_core.getAdapter()->VirtualQueryExT(currentAddr, &mbi) != STATUS_SUCCESS) {
					currentAddr += ArchitectureApi().GetPageSize();
					continue;
				}
				if (mbi.State != MEM_COMMIT || (mbi.Protect & PAGE_NOACCESS) == PAGE_NOACCESS) {
					currentAddr = mbi.BaseAddress + mbi.RegionSize;
					continue;
				}

				ptr_t readStart = currentAddr;
				size_t readSize = (mbi.RegionSize - (readStart - mbi.BaseAddress)) < ((baseAddr + dwSize) - readStart) ? 
					(mbi.RegionSize - (readStart - mbi.BaseAddress)) : ((baseAddr + dwSize) - readStart);

				ptr_t memoffset = readStart - baseAddr;
				NTSTATUS status = _core.getAdapter()->ReadProcessMemoryT(readStart, (LPVOID)((BYTE*)pResult + memoffset), readSize, &dwRead);

				if (!NT_SUCCESS(status)) {
					return status;
				}

				currentAddr = readStart + readSize;

				if (dwRead != readSize) {
					return STATUS_PARTIAL_COPY;
				}
			}
		}
		return STATUS_SUCCESS;
	}

	//addr list read - pointer chain traversal
	MEMX_API NTSTATUS ProcessMemory::Read(const std::vector<ptr_t>& addrList, PVOID pResult, size_t dwSize, bool skipUncommited) {
		if (addrList.empty()) {
			return STATUS_INVALID_PARAMETER;
		}

		if (addrList.size() == 1) {
			return Read(addrList[0], pResult, dwSize, skipUncommited);
		}

		ptr_t currentAddr = addrList[0];
		ptr_t pointerValue = 0;

		for (size_t i = 1; i < addrList.size(); i++) {
			NTSTATUS status = _core.getAdapter()->ReadProcessMemoryT(currentAddr, &pointerValue, sizeof(ptr_t), nullptr);
			if (!NT_SUCCESS(status)) {
				return status;
			}
			currentAddr = pointerValue + addrList[i];
		}
		return Read(currentAddr, pResult, dwSize, skipUncommited);
	}

	MEMX_API NTSTATUS ProcessMemory::Write(ptr_t baseAddr, LPCVOID pData, size_t dwSize) {
		if ( !baseAddr ) return STATUS_INVALID_ADDRESS;
		return _core.getAdapter()->WriteProcessMemoryT(baseAddr, pData, dwSize);
	}


	//addr list write - pointer chain traversal
	MEMX_API NTSTATUS ProcessMemory::Write(const std::vector<ptr_t>& addrList, LPCVOID pData, size_t dwSize) {
		if ( addrList.empty() ) {
			return STATUS_INVALID_PARAMETER;
		}

		if ( addrList.size() == 1 ) {
			return Write(addrList[ 0 ], pData, dwSize);
		}

		ptr_t currentAddr = addrList[ 0 ];
		ptr_t pointerValue = 0;

		for ( size_t i = 1; i < addrList.size(); i++ ) {
			NTSTATUS status = _core.getAdapter()->ReadProcessMemoryT(currentAddr, &pointerValue, sizeof(ptr_t), nullptr);
			if ( !NT_SUCCESS(status) ) {
				return status;
			}
			currentAddr = pointerValue + addrList[ i ];
		}
		return Write(currentAddr, pData, dwSize);
	}
}