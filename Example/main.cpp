#include <iostream>
#include <MemX/Process/Process.h>
#include <MemX/Common/WinApi/NtResult.h>

int main() {
	MemX::Process process;
	process.Catch(0x1d40);

	if ( process.Core().isActive() ) {
		MemX::NtResult<DWORD> result = process.Memory().Read<DWORD>(0x600B4130);
		if ( result.success() ) {
			process.Memory().Write<DWORD>(0x600B4130, 123456789);
		}
		std::cout << "Read Result: " << std::dec << result.result() << std::endl;
	}

	return 0;
}