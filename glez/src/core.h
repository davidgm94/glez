#pragma once
#ifdef GLEZ_PLATFORM_WINDOWS
	#ifdef GLEZ_BUILD_DLL
		#define GLEZ_API __declspec(dllexport)
	#else
		#define GLEZ_API __declspec(dllimport)
	#endif
#else
#pragma error "Windows is the only platform supported at the moment"
#endif
