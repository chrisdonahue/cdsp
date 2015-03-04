#ifndef CDSP_PRIMITIVES_OSCILLATORS_PHASOR
#define CDSP_PRIMITIVES_OSCILLATORS_PHASOR

namespace cdsp { namespace primitives { namespace oscillators {
	class phasor : public primitive::base {
	public:
		phasor(types::sample phase, types::sample frequency);

		void prepare(types::cont_64 _sample_rate, types::disc_32_u _block_size);
		void perform(sample_buffer& buffer, types::disc_32_u block_size_leq, types::channel offset_channel = 0, types::disc_32_u offset_sample = 0);

		void phase_set(types::cont_64 _phase);
		  
		void frequency_set(types::sample frequency);
		void frequency_next_set(types::sample frequency_next);
		void frequency_next_set(types::sample frequency_next, types::cont_64 delay_s);

	private:
		types::cont_64 phase;
		parameter::ramp_linear_automatic frequency;
	};
}}}

#endif