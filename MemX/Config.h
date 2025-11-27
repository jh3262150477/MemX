#pragma once

#if !defined(MEMX_EXPORTS) && !defined(MEMX_IMPORTS) && !defined(MEMX_STATIC)
#define MEMX_STATIC
#endif //Lib/Dll switch

#if defined(_MSC_VER) || defined(__clang__)
	#ifndef COMPILER_MSVC
		#define COMPILER_MSVC 1
	#endif
	#if defined(MEMX_IMPORTS)
		#define MEMX_API __declspec(dllimport)
	#elif defined(MEMX_EXPORTS)
		#define MEMX_API __declspec(dllexport)
	#else
		#define MEMX_API
	#endif

#elif defined(__GUNC__)
	#define COMPILER_GCC
	#define MEMX_API

#else
	#error "Unsupported compiler"
#endif