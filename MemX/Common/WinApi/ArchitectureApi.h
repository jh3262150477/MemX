#pragma once
#include "../Types/Type.h"
#include "../WinApi/WinHeaders.h"

namespace MemX {
	class ArchitectureApi {
		public:
		ArchitectureApi() {
			SYSTEM_INFO sysInfo = { 0 };
			GetNativeSystemInfo(&sysInfo);
			_dwPageSize = sysInfo.dwPageSize;
		}

		ArchitectureApi(HANDLE _hProcess) : ArchitectureApi() {
			BOOL isSourceWow64 = FALSE, isTargetWow64 = FALSE;
			IsWow64Process(GetCurrentProcess(), (BOOL*) &isSourceWow64);
			IsWow64Process(_hProcess, (BOOL*) &isTargetWow64);

			_wow64Barrier.sourceWow64 = (isSourceWow64 == TRUE);
			_wow64Barrier.targetWow64 = (isTargetWow64 == TRUE);

			if ( isSourceWow64 == TRUE && isTargetWow64 == TRUE ) {
				_wow64Barrier.archType = ARCHITECTURE_32_32;
			} else if ( isSourceWow64 == TRUE && isTargetWow64 == FALSE ) {
				_wow64Barrier.archType = ARCHITECTURE_64_32;
			} else if ( isSourceWow64 == FALSE && isTargetWow64 == FALSE ) {
				_wow64Barrier.archType = ARCHITECTURE_64_64;
			} else {
				_wow64Barrier.archType = ARCHITECTURE_32_64;
			}
		}
		~ArchitectureApi() {

		}

		const Wow64Barrier& GetWow64Barrier() const { return _wow64Barrier; }
		uint32_t GetPageSize() const { return _dwPageSize; }

		private:
		Wow64Barrier _wow64Barrier;
		uint32_t _dwPageSize;
	};
}