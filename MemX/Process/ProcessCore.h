#pragma once
#include "../Common/WinApi/WinHeaders.h"
#include "../Adapter/Adapter.h"

#include <memory>


namespace MemX {
	class ProcessCore {
		public:
		MEMX_API NTSTATUS isWow64() const { return NULL; };// TODO: Implement isWow64 check
		MEMX_API DWORD getPid() const { return _pid; };
		MEMX_API HANDLE getHandle() const { return this->_hProcess; };
		MEMX_API Adapter* getAdapter() const { return _adapter.get(); };
		MEMX_API BOOL isActive() const { return _active; };
		

		private:
		friend class Process;
		using ptrAdapter = std::unique_ptr<Adapter>;

		private:
		ProcessCore();
		ProcessCore(const Process& process) = delete;
		~ProcessCore();

		NTSTATUS Open(DWORD pid, DWORD access);

		NTSTATUS Open(HANDLE proHandle);

		NTSTATUS Init();

		void Close();

		private:
		HANDLE _hProcess = nullptr;
		DWORD _pid = 0;
		BOOL _active = FALSE;
		ptrAdapter _adapter = nullptr;
	};
}