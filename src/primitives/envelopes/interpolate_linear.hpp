#ifndef CDSP_PRIMITIVES_ENVELOPES_INTERPOLATE_LINEAR
#define CDSP_PRIMITIVES_ENVELOPES_INTERPOLATE_LINEAR

#include <tuple>

#include "envelopes_base.hpp"

namespace cdsp { namespace primitives { namespace envelopes {
	template <types::disc_32_u points_num>
	class interpolate_linear : public base<points_num> {
	public:
		interpolate_linear() :
			base<points_num>(),
			points_increments(points_num),
			point_samples_remaining(0),
			point_increment_current(values::sample_zero)
		{};

		interpolate_linear(types::sample _value_initial) :
			base<points_num>(_value_initial),
			points_increments(points_num),
			point_samples_remaining(0),
			point_increment_current(values::sample_zero)
		{};

		void perform(sample_buffer& buffer, types::disc_32_u block_size_leq, types::channel offset_channel = 0, types::index offset_sample = 0) {
			base<points_num>::perform(buffer, block_size_leq, offset_channel, offset_sample);

			types::sample* output = buffer.channel_pointer_write(offset_channel, offset_sample);
			types::index samples_remaining = block_size_leq;

			// holding
			if (points_index_current >= points.size()) {
				while (samples_remaining--) {
					*(output++) = value;
				}
			}
			// ramping
			else {
				while (samples_remaining--) {
					if (point_samples_remaining <= 0) {
						value = points.at(points_index_current).second;
						points_index_current += 1;
						if (points_index_current == points.size()) {
							point_samples_remaining = 0;
							point_increment_current = values::sample_zero;
						}
						else {
							point_samples_remaining = points_increments.at(points_index_current).first;
							point_increment_current = points_increments.at(points_index_current).second;
						}
					}

					*(output++) = value;
					value += point_increment_current;
					point_samples_remaining--;
				}
			}
		};
	
		void point_set(types::disc_32_u point_index, types::time length_s, types::sample value) {
			base<points_num>::point_set(point_index, length_s, value);

			// calculate length of segment in samples
			types::index point_length = static_cast<types::index>(length_s * sample_rate);

			// calculate linear increment
			types::sample point_increment;
			if (point_index == 0) {
				point_increment = (value - value_initial)/static_cast<types::sample>(point_length);
				point_samples_remaining = point_length;
				point_increment_current = point_increment;
			}
			else {
				point_increment = (value - points.at(point_index - 1).second) / static_cast<types::sample>(point_length);
			}

			// add point increment
			points_increments[point_index] = std::make_pair(point_length, point_increment);
		};

	private:
		std::vector<std::pair<types::index, types::sample> > points_increments;
		types::index point_samples_remaining;
		types::sample point_increment_current;
	};
}}}

#endif