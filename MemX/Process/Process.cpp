#include "Process.h"

namespace MemX {
    Process::Process() :_core(), _memory(this) {
    }
    Process::~Process(void) {
    }

    MEMX_API NTSTATUS Process::Catch(DWORD pid, DWORD access) {
        Drop();
        return _core.Open(pid, access);
    }

    MEMX_API NTSTATUS Process::Catch(const wchar_t* processName, DWORD access) {
		//TODO: Implement process name to PID resolution
		return STATUS_NOT_IMPLEMENTED;
    }

    MEMX_API NTSTATUS Process::Catch(HANDLE proHandle) {
        Drop();
        return _core.Open(proHandle);
    }
    MEMX_API NTSTATUS Process::Drop() {
		_core.Close();

		return STATUS_SUCCESS;
    }
}
