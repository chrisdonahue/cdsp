#ifndef CDSP_VALUES
#define CDSP_VALUES

#include <limits>

#include "defines.hpp"
#include "types.hpp"

namespace cdsp { namespace values {
	// pi
	const types::cont_64 pi_64					= 3.1415926535897932384626433832795028841971693993751;
	const types::cont_32 pi_32					= 3.14159265358979f;
	const types::cont_64 two_pi_64				= 6.2831853071795864769252867665590057683943387987502;
	const types::cont_32 two_pi_32				= 6.283185307179586f;

	// zero
	const types::cont_32 zero_32				= 0.0f;
	const types::cont_64 zero_64				= 0.0;

	// one
	const types::cont_32 one_32					= 1.0f;
	const types::cont_64 one_64					= 1.0;

	// infinity
	const types::cont_32 infinity_n_32			= -1.0f * std::numeric_limits<types::cont_32>::infinity();
	const types::cont_32 infinity_32			= std::numeric_limits<types::cont_32>::infinity();
	const types::cont_64 infinity_n_64			= -1.0f * std::numeric_limits<types::cont_64>::infinity();
	const types::cont_64 infinity_64			= std::numeric_limits<types::cont_64>::infinity();

	// nan
#ifdef CDSP_DEBUG_DSP
	const types::cont_32 nan_32					= std::numeric_limits<types::cont_32>::signaling_NaN();
	const types::cont_64 nan_64					= std::numeric_limits<types::cont_64>::signaling_NaN();
#else
	const types::cont_32 nan_32					= std::numeric_limits<types::cont_32>::quiet_NaN();
	const types::cont_64 nan_64					= std::numeric_limits<types::cont_64>::quiet_NaN();
#endif

	// channels
	const types::channel channel_zero			= static_cast<types::channel>(0);
	const types::channel channel_max	 		= std::numeric_limits<types::channel>::max();

	// index
	const types::index index_zero				= static_cast<types::index>(0);

#ifdef CDSP_SAMPLE_32
	const types::sample sample_zero				= static_cast<types::sample>(0.0f);
	const types::sample sample_one				= static_cast<types::sample>(1.0f);
	const types::sample sample_silence			= static_cast<types::sample>(0.0f);
	const types::sample sample_line_level_n		= static_cast<types::sample>(-1.0f);
	const types::sample sample_line_level		= static_cast<types::sample>(1.0f);
	const types::sample sample_min				= std::numeric_limits<types::sample>::min();
	const types::sample sample_max				= std::numeric_limits<types::sample>::max();
	const types::sample sample_infinity_n		= -1.0f * std::numeric_limits<types::sample>::infinity();
	const types::sample sample_infinity			= std::numeric_limits<types::sample>::infinity();

	const types::sample identity_additive		= static_cast<types::sample>(0.0f);
	const types::sample identity_multiplicative	= static_cast<types::sample>(1.0f);

	const types::sample sample_dc_block_r		= static_cast<types::sample>(0.995f);
#elif CDSP_SAMPLE_64
	const types::sample sample_silence			= static_cast<types::sample>(0.0);
	const types::sample sample_line_level_n		= static_cast<types::sample>(-1.0);
	const types::sample sample_line_level		= static_cast<types::sample>(1.0);
	const types::sample sample_infinity_n		= -1.0 * std::numeric_limits<types::sample>::infinity();
	const types::sample sample_infinity			= std::numeric_limits<types::sample>::infinity();

	const types::sample sample_dc_block_r		= static_cast<types::sample>(0.995);
#endif
}}

#endif