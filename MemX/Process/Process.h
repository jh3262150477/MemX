#pragma once
#include "ProcessMemory.h"
#include "ProcessCore.h"
#include "../Common/WinApi/WinHeaders.h"
#include "../Common/WinApi/NtResult.h"

#include <string>
#include <list>


namespace MemX {

	//<summary>
	// Contains information about a process.
	//</summary>
	struct ProcessInfo {
		uint32_t pid = 0;
		std::wstring imageName;

		bool operator < (const ProcessInfo& other) const {
			return this->pid < other.pid;
		}
	};

	/// <summary>
	/// Handle information
	/// </summary>
	struct HandleInfo {
		HANDLE handle = nullptr;
		uint32_t access = 0;
		uint32_t flags = 0;

		std::wstring name;
	};

	#define DEFAULT_NORMAL_PROCESS_ACCESS \
									 PROCESS_VM_READ		   |\
                                	 PROCESS_VM_WRITE          |\
									 PROCESS_VM_OPERATION      |\
									 PROCESS_QUERY_INFORMATION |\
									 PROCESS_CREATE_THREAD
	 

	#define DEFAULT_ALL_PROCESS_ACCESS  \
	                          PROCESS_QUERY_INFORMATION | \
							  PROCESS_VM_READ           | \
                              PROCESS_VM_WRITE          | \
                              PROCESS_VM_OPERATION      | \
                              PROCESS_CREATE_THREAD     | \
                              PROCESS_SET_QUOTA         | \
                              PROCESS_TERMINATE         | \
                              PROCESS_SUSPEND_RESUME    | \
                              PROCESS_DUP_HANDLE

	class Process {
		public:
		MEMX_API Process();
		MEMX_API ~Process(void);

		
		MEMX_API NTSTATUS Catch(DWORD pid, DWORD access = DEFAULT_NORMAL_PROCESS_ACCESS);

		MEMX_API NTSTATUS Catch(const wchar_t* processName, DWORD access = DEFAULT_NORMAL_PROCESS_ACCESS);

		MEMX_API NTSTATUS Catch(HANDLE proHandle);

		MEMX_API NTSTATUS Drop();

		MEMX_API ProcessMemory& Memory() { return _memory; }
		MEMX_API ProcessCore& Core() { return _core; }

		private:
		Process(const Process&) = delete;
		Process& operator =(const Process&) = delete;


		private:
		// Ensure ProcessCore is declared before ProcessMemory so it is constructed first.
		ProcessCore _core;
		ProcessMemory _memory;
	};
}
