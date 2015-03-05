#ifndef CDSP_BASE
#define CDSP_BASE

#include "defines.hpp"
#include "exceptions.hpp"
#include "sample_buffer.hpp"
#include "types.hpp"

namespace cdsp {
	class base {
	protected:
		types::string identifier;
	};

	class dsp : public base {
	public:
		dsp() :
			sample_rate(values::nan_64),
			sample_rate_inverse(values::nan_64),
			block_size(0),
			block_size_inverse(values::nan_64)
		{
#ifdef CDSP_DEBUG
			prepared = false;
#endif
		};

		virtual void prepare(types::cont_64 _sample_rate, types::disc_32_u _block_size) {
			sample_rate = _sample_rate;
			sample_rate_inverse = values::one_64 / sample_rate;
			block_size = _block_size;
			block_size_inverse = values::one_64 / static_cast<types::cont_64>(block_size);
#ifdef CDSP_DEBUG
			prepared = true;
#endif
		}
		virtual void release() {
#ifdef CDSP_DEBUG
			prepared = false;
#endif
		};
		virtual void perform(sample_buffer& buffer, types::disc_32_u block_size_leq, types::channel offset_channel = 0, types::index offset_sample = 0) = 0;

	protected:
		types::cont_64 sample_rate;
		types::cont_64 sample_rate_inverse;
		types::disc_32_u block_size;
		types::cont_64 block_size_inverse;

	private:
#ifdef CDSP_DEBUG
		types::boolean prepared;
#endif
	};
}

#endif
