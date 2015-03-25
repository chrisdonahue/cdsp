#ifndef CDSP_DEFINE
#define CDSP_DEFINE

// DEBUG FLAG
#define CDSP_DEBUG_API
#define CDSP_DEBUG_INTERNAL
#define CDSP_DEBUG_DSP

// OS FLAGS
#ifdef _WIN32
	#define CDSP_WIN
	#ifdef _WIN64
		#define CDSP_WIN_64
	#else
		#define CDSP_WIN_32
	#endif
#elif __APPLE__
	#define CDSP_MAC
#elif __linux
	#define CDSP_LINUX
#elif __unix
	#define CDSP_UNIX
#elif __posix
	#define CDSP_POSIX
#endif

// SAMPLE BIT DEPTH FLAG
#define CDSP_SAMPLE_32
//#define CDSP_SAMPLE_64

// MEMORY FLAG
//#define CDSP_MEMORY_LOW
#define CDSP_MEMORY_HIGH

// UNUSED PARAMETER
#define CDSP_UNUSED_PARAMETER(P) P;

#endif