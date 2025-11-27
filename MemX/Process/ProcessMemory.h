#pragma once

#include "../Common/WinApi/WinHeaders.h"
#include "../Common/Types/Type.h"
#include "../Common/WinApi/NtResult.h"
#include "../Config.h"

#include <vector>

namespace MemX {


	class ProcessMemory {
		public:
		MEMX_API ProcessMemory(class Process* process);
		MEMX_API ~ProcessMemory();

		MEMX_API NTSTATUS Read(ptr_t baseAddr, PVOID pResult, size_t dwSize, bool skipUncommited);

		MEMX_API NTSTATUS Read(const std::vector<ptr_t>& addrList, PVOID pResult, size_t dwSize, bool skipUncommited);

		MEMX_API NTSTATUS Write(ptr_t baseAddr, LPCVOID pData, size_t dwSize);

		MEMX_API NTSTATUS Write(const std::vector<ptr_t>& addrList, LPCVOID pData, size_t dwSize);

		template<typename T>
		NtResult<T> Read(ptr_t baseAddr) {
			T* pResult = new T();
			NTSTATUS status = Read(baseAddr, pResult, sizeof(T), false);
			return NtResult<T>(*pResult, status);
		}

		template<typename T>
		NtResult<T> Read(std::vector<ptr_t>&& addList) {
			T* pResult = new T();
			NTSTATUS status = Read(std::forward<std::vector<ptr_t>>(addList), pResult, sizeof(T), false);
			return NtResult<T>(*pResult, status);
		}


		template<typename T>
		NTSTATUS Write(ptr_t baseAddr, const T& data) {
			return Write(baseAddr, reinterpret_cast<LPCVOID>(&data), sizeof(T));
		}

		template<typename T>
		NTSTATUS Write(const std::vector<ptr_t>&& addrList, const T& data) {
			return Write(std::forward<std::vector<ptr_t>>(addrList), reinterpret_cast<LPCVOID>(&data), sizeof(T));
		}



		private:
		ProcessMemory(const ProcessMemory&) = delete;
		ProcessMemory& operator=(const ProcessMemory&) = delete;

		private:
		class Process* _process;
		class ProcessCore& _core;
	};
}