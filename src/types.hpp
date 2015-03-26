#ifndef CDSP_TYPE
#define CDSP_TYPE

#include <cstdint>
#include <map>
#include <string>
#include <unordered_map>

#include "define.hpp"

namespace cdsp { namespace types {
	typedef uint32_t disc_32_u;
	typedef uint64_t disc_64_u;
	typedef int32_t disc_32;
	typedef int64_t disc_64;
	typedef float cont_32;
	typedef double cont_64;
	typedef bool boolean;
	typedef size_t size;
#ifdef CDSP_SAMPLE_64
	typedef cont_64 sample;
#else
	typedef cont_32 sample;
#endif
	typedef cont_64 time;
	typedef disc_32_u index;
	typedef disc_32_u channel;
	typedef std::string string;
}}

#endif
