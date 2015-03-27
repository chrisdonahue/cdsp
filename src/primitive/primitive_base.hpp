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
		base() : dsp(),
			channels_input_num(0),
			channels_output_num(0)
		{};

		// dsp
		virtual void prepare(prepare_signature) {
			dsp::prepare(prepare_arguments);
		};
		virtual void release() {
			dsp::release();
		};
		virtual void perform(perform_signature_defaults) = 0;

		// channels
		types::channel channels_input_num_get() {
			return channels_input_num;
		};

		types::channel channels_output_num_get() {
			return channels_output_num;
		};

	protected:
		void channels_input_num_set(types::channel _channels_input_num) {
			channels_input_num = _channels_input_num;
		};

		void channels_output_num_set(types::channel _channels_output_num) {
			channels_output_num = _channels_output_num;
		};

		types::channel channels_input_num;
		types::channel channels_output_num;
	};

	// need this class as base class for parameterized otherwise each would contain a set of types::string
	class _parameterized : public base {
	protected:
		// TODO: have this return an iterator
		const std::set<types::string>& parameter_specifiers_get() {
			return const_cast<std::set<types::string>& >(parameter_specifiers);
		};

		void parameter_specifier_add(types::string parameter_specifier) {

		};

		void parameter_specifier_remove(types::string parameter_specifier) {

		};

	private:
		std::set<types::string> parameter_specifiers;
	};

	template <class P>
	class parameterized : virtual public _parameterized {
		parameterized() : _parameterized() {};

		P& parameter_get(types::string parameter_specifier) {
			auto it = parameters_rate_block.find(parameter_specifier);
			types::boolean parameter_specifier_exposed = it != parameter_specifiers_pluggable.end();

#ifdef CDSP_DEBUG_API
			if (!parameter_specifier_exposed) {
				throw cdsp::exception::runtime("no parameter with this specifier was registered by this primitive");
			}
#endif

			return *(it->second);
		};

	protected:
		void parameter_expose(types::string parameter_specifier, P& parameter) {
			parameterized::parameter_specifier_expose(parameter_specifier);
			parameters.insert(std::make_pair(parameter_specifier, parameter));
		};

	private:
		std::unordered_map<types::string, P* > parameters;
	};

	/*
	class parameterized_rate_audio : public parameterized {
	public:
		parameterized_rate_audio() : parameterized() {};

		// dsp
		parameter::rate_audio::base& parameter_get(types::string parameter_specifier) {
			auto it = parameters_rate_audio.find(parameter_specifier);
			types::boolean parameter_specifier_exposeed = it != parameters_rate_audio.end();

#ifdef CDSP_DEBUG_API
			if (!parameter_specifier_exposeed) {
				throw cdsp::exception::runtime("no audio-rate parameter with this specifier was registered by this primitive");
			}
#endif

			return *(it->second);
		};

	protected:
		void parameter_expose(types::string parameter_specifier, parameter::rate_audio::base& parameter) {
			parameterized::parameter_specifier_expose(parameter_specifier);
			parameters_rate_audio.insert(std::make_pair(parameter_specifier, &parameter));
		};

	private:
		std::unordered_map<types::string, parameter::rate_audio::base* > parameters_rate_audio;
	};

	class parameterized_rate_audio_pluggable : public parameterized_rate_audio {
	public:
		parameterized_rate_audio_pluggable() : parameterized_rate_audio() {};

		// dsp
		virtual void prepare(prepare_signature) {
			parameterized_rate_audio::prepare(prepare_arguments);
			for (auto it : parameters_rate_audio_plugged) {
				it.second->prepare(prepare_arguments);
			}
		};
		virtual void release() {
			parameterized_rate_audio::release();
			for (auto it : parameters_rate_audio_plugged) {
				it.second->release();
			}
		};

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
	*/
}}

#endif
