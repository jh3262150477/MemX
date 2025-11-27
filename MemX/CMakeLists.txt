cmake_minimum_required (VERSION 3.10)

project ("MemXLibs")

set(CXX_STANDARD 17)

############################################################
set(HEADER_INCLUDE "Common/WinApi/WinHeaders.h"
					"Common/WinApi/NtResult.h"
					"Common/WinApi/NtError.h"
					"Common/WinApi/ArchitectureApi.h"
					"Common/Types/Type.h")

file(GLOB Inc ${HEADER_INCLUDE})
source_group(Inc FILES ${Inc})
############################################################
set(SOURCE_PROCESS "Process/Process.h"
					"Process/ProcessMemory.h"
					"Process/ProcessCore.h")

set(HEADER_PROCESS "Process/Process.cpp"
					"Process/ProcessMemory.cpp"
					"Process/ProcessCore.cpp")

file(GLOB Process ${SOURCE_PROCESS} ${HEADER_PROCESS})
source_group(Process FILES ${Process})
#############################################################
set(HEADER_ADAPTER "Adapter/Adapter.h")
set(SOURCE_ADAPTER "Adapter/Adapter.cpp")
file(GLOB Adapter ${HEADER_ADAPTER} ${SOURCE_ADAPTER})
source_group(Adapter FILES ${Adapter})
#############################################################
set(HEADER_MAIN "Config.h")
#############################################################
set(SOURCE_LIB ${SOURCE_PROCESS}
				${HEADER_ADAPTER})
set(HEADER_LIB ${HEADER_PROCESS}
				${HEADER_INCLUDE}
				${SOURCE_ADAPTER}
				${HEADER_MAIN})

add_library(MemX STATIC ${SOURCE_LIB} ${HEADER_LIB})