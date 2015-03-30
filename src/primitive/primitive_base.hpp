#ifndef CDSP_PRIMITIVE_BASE
#define CDSP_PRIMITIVE_BASE

#include <set>
#include <unordered_map>
#include <utility>

#include "../exception.hpp"
#include "../parameter.hpp"
#include "../sample_buffer.hpp"
#include "../types.hpp"
#include "../values.hpp"

namespace cdsp { namespace primitive {
	class base : public dsp {
	public:
		virtual void perform(perform_signature_defaults) = 0;

		// channels
		types::channel channels_input_num_get() {
			return channels_input_num;
		};

		types::channel channels_output_num_get() {
			return channels_output_num;
		};

	protected:
		base() : dsp(),
			channels_input_num(0),
			channels_output_num(0)
		{};

		base(types::channel _channels_input_num, types::channel _channels_output_num) : dsp(),
			channels_input_num(_channels_input_num),
			channels_output_num(_channels_output_num)
		{};

		void channels_input_num_set(types::channel _channels_input_num) {
			channels_input_num = _channels_input_num;
		};

		void channels_output_num_set(types::channel _channels_output_num) {
			channels_output_num = _channels_output_num;
		};

		types::channel channels_input_num;
		types::channel channels_output_num;
	};

	class pluggable : public base {
	public:
		pluggable() : base() {};
		pluggable(types::channel _channels_input_num, types::channel _channels_output_num) : base(_channels_input_num, _channels_output_num) {};

		// TODO: have this return an iterator
		const std::set<types::string>& parameter_specifiers_pluggable_get() {
			return const_cast<std::set<types::string>& >(parameter_specifiers_pluggable);
		};

		// plugs
		void parameter_plug(types::string parameter_specifier, parameter::rate_audio::signal& parameter_plug) {
			types::boolean parameter_specifier_pluggable = parameter_specifiers_pluggable.find(parameter_specifier) != parameter_specifiers_pluggable.end();

#ifdef CDSP_DEBUG_API
			if (!parameter_specifier_pluggable) {
				throw cdsp::exception::runtime("tried to plug a parameter specifier that was not registered as pluggable by this primitive");
			}
#endif

			parameters_rate_audio_plugged.insert(std::make_pair(parameter_specifier, &parameter_plug));
		};

		void parameter_unplug(types::string parameter_specifier) {
			parameters_rate_audio_plugged.erase(parameter_specifier);
		};

	protected:
		void parameter_expose_pluggable(types::string parameter_specifier) {
			parameter_specifiers_pluggable.insert(parameter_specifier);
		};

		types::boolean parameter_plugged(types::string parameter_specifier) {
			return parameters_rate_audio_plugged.find(parameter_specifier) != parameters_rate_audio_plugged.end();
		};

		parameter::rate_audio::signal& parameter_plugged_get(types::string parameter_specifier) {
			auto it = parameters_rate_audio_plugged.find(parameter_specifier);
			types::boolean parameter_plugged = it != parameters_rate_audio_plugged.end();

#ifdef CDSP_DEBUG_INTERNAL
			cdsp::ensure(parameter_plugged);
#endif

			return *(it->second);
		};

	private:
		std::set<types::string> parameter_specifiers_pluggable;
		std::unordered_map<types::string, parameter::rate_audio::signal* > parameters_rate_audio_plugged;
	};
}}

#endif
