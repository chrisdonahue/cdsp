#ifndef CDSP_PRIMITIVE_BASE
#define CDSP_PRIMITIVE_BASE

#include <set>
#include <unordered_map>
#include <utility>

#include "../exceptions.hpp"
#include "../parameter.hpp"
#include "../sample_buffer.hpp"
#include "../types.hpp"
#include "../values.hpp"

namespace cdsp { namespace primitives {
	class base : public dsp {
	public:
		base() : dsp(),
			channels_input_num(0),
			channels_output_num(0)
		{
		};

		// dsp
		virtual void prepare(types::cont_64 _sample_rate, types::disc_32_u _block_size) {
			dsp::prepare(_sample_rate, _block_size);
			for (auto it : parameter_plugs) {
				(it.second)->prepare(_sample_rate, _block_size);
			}
		};
		virtual void release() {
			dsp::release();
			for (auto it : parameter_plugs) {
				(it.second)->release();
			}
		};
		virtual void perform(sample_buffer& buffer, types::disc_32_u block_size_leq, types::channel offset_channel = 0, types::index offset_sample = 0) = 0;

		// channels
		types::channel channels_input_num_get() {
			return channels_input_num;
		};

		types::channel channels_output_num_get() {
			return channels_output_num;
		};

		// plugs
		void parameter_plug(types::string parameter_specifier, parameter::signal* parameter_plug) {
			parameter_plugs.insert(std::make_pair(parameter_specifier, parameter_plug));
		};

		void parameter_unplug(types::string parameter_specifier) {
			parameter_plugs.erase(parameter_specifier);
		};

		const std::set<types::string>& parameter_plug_specifiers_get() {
			return const_cast<std::set<types::string>& >(parameter_plug_specifiers);
		};

	protected:
		void channels_input_num_set(types::channel _channels_input_num) {
			channels_input_num = _channels_input_num;
		};

		void channels_output_num_set(types::channel _channels_output_num) {
			channels_output_num = _channels_output_num;
		};

		void parameter_plug_register(types::string parameter_specifier) {
			// TODO: throw runtime error if already exists
			parameter_plug_specifiers.insert(parameter_specifier);
		};

		parameter::signal* parameter_plug_get(types::string parameter_specifier) {
			auto it = parameter_plugs.find(parameter_specifier);
			if (it != parameter_plugs.end()) {
				return it->second;
			}
			else {
				return nullptr;
			}
		};

	private:
		types::channel channels_input_num;
		types::channel channels_output_num;

		std::set<types::string> parameter_plug_specifiers;
		std::unordered_map<types::string, parameter::signal*> parameter_plugs;
	};
}}

#endif
