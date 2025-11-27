#pragma once
#include <cstdint>

namespace MemX {
	using ptr_t = uintptr_t;

	enum ArchBarrier {
		ARCHITECTURE_32_32,//Both processes are Wow64
		ARCHITECTURE_64_64,//Both processes are x64
		ARCHITECTURE_64_32,//Control Wow64 process from x64;
		ARCHITECTURE_32_64,//Control x64 process from Wow64 
		ARCHITECTURE_UNSUPPORTED
	};

	struct Wow64Barrier {
		ArchBarrier archType = ARCHITECTURE_UNSUPPORTED;
		bool sourceWow64 = false;
		bool targetWow64 = false;
		bool mismatch = false;
	};
}
