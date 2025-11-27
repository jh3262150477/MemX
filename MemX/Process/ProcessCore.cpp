#include "ProcessCore.h"
#include "../Common/WinApi/NtError.h"
#include "../Common/WinApi/ArchitectureApi.h"
#include <VersionHelpers.h>

namespace MemX {
	ProcessCore::ProcessCore() : _adapter(nullptr) {

	}

	ProcessCore::~ProcessCore() {

	}

	NTSTATUS ProcessCore::Open(DWORD pid, DWORD access) {
		if ( pid == GetCurrentProcessId() ) {
			_hProcess = GetCurrentProcess();
			_active = TRUE;
		} else {
			_hProcess = OpenProcess(access, FALSE, pid);
		}

		//Some Windows 10 versions require PROCESS_ALL_ACCESS for current process handle
		if ( IsWindows10OrGreater() && pid == GetCurrentProcessId() ) {
			_hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
		}


		if ( _hProcess ) {
			_pid = pid;
			_active = TRUE;
			return Init();
		}

		return GetLastNtStatus();
	}

	NTSTATUS ProcessCore::Open(HANDLE proHandle) {
		_hProcess = proHandle;
		_pid = GetProcessId(_hProcess);

		if ( IsWindows10OrGreater() && _pid == GetCurrentProcessId() )
			_hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, _pid);

		if ( _hProcess ) {
			_active = TRUE;
		}

		return Init();
	}

	NTSTATUS ProcessCore::Init() {
		ArchitectureApi texApi(_hProcess);
		BOOL wowSrc = texApi.GetWow64Barrier().sourceWow64;
		if ( !wowSrc ) {
			_adapter = std::make_unique<Adapter>(_hProcess);
		}
		return STATUS_SUCCESS;
	}

	void ProcessCore::Close() {
		CloseHandle(_hProcess);
		_adapter.reset();
		_pid = 0;
	}
}