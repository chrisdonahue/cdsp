#ifndef CDSP_DSP
#define CDSP_DSP

#define prepare_signature types::cont_64 _sample_rate, types::index _block_size
#define prepare_arguments _sample_rate, _block_size
#define perform_signature sample_buffer& buffer, types::index block_size_leq, types::channel offset_channel, types::index offset_sample
#define perform_signature_defaults sample_buffer& buffer, types::index block_size_leq, types::channel offset_channel = 0, types::index offset_sample = 0
#define perform_arguments buffer, block_size_leq, offset_channel, offset_sample

#include <assert.h>

#include "define.hpp"
#include "exception.hpp"
#include "sample_buffer.hpp"
#include "types.hpp"

namespace cdsp {
	class dsp {
	public:
		dsp() :
			sample_rate(values::nan_64),
			sample_rate_inverse(values::nan_64),
			block_size(0),
			block_size_inverse(values::nan_64)
		{
#if defined(CDSP_DEBUG_DSP) || defined(CDSP_DEBUG_API)
			prepared = false;
#endif
		};

		virtual void prepare(prepare_signature) {
			sample_rate = _sample_rate;
			sample_rate_inverse = values::one_64 / sample_rate;
			block_size = _block_size;
			block_size_inverse = values::one_64 / static_cast<types::cont_64>(block_size);
#if defined(CDSP_DEBUG_DSP) || defined(CDSP_DEBUG_API)
			prepared = true;
#endif
		};

		virtual void release() {
#if defined(CDSP_DEBUG_DSP) || defined(CDSP_DEBUG_API)
			prepared = false;
#endif
		};

		virtual void perform(perform_signature_defaults) = 0 {
#ifdef CDSP_DEBUG_DSP
			cdsp::ensure(prepared);
#endif
		};

	protected:
		types::cont_64 sample_rate;
		types::cont_64 sample_rate_inverse;
		types::index block_size;
		types::cont_64 block_size_inverse;
#if defined(CDSP_DEBUG_DSP) || defined(CDSP_DEBUG_API)
		types::boolean prepared;
#endif
	};
}

#endif
