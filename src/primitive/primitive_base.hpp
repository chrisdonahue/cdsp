#ifndef CDSP_PRIMITIVE_BASE
#define CDSP_PRIMITIVE_BASE

#include <unordered_map>

#include "../dsp.hpp"
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

		// TODO: confirm required pug registers in PREPARE and PERFORM

		// TODO: have this return an iterator
		/*
		const std::set<types::string>& parameter_specifiers_pluggable_get() {
			return const_cast<std::set<types::string>& >(parameter_specifiers_pluggable);
		};
		*/

		// plugs
		void plug(types::string parameter_specifier, types::channel channel) {
			auto it = plugs.find(parameter_specifier);
			types::boolean pluggable = it != plugs.end();

#ifdef CDSP_DEBUG_API
			cdsp::ensure(pluggable);
#endif

			plugged_currently.insert(parameter_specifier);
			it->second.channel_set(channel);
		};

		void unplug(types::string parameter_specifier) {
			auto it = plugs.find(parameter_specifier);
			types::boolean pluggable = it != plugs.end();

#ifdef CDSP_DEBUG_API
			cdsp::ensure(pluggable);
#endif

			plugged_currently.erase(parameter_specifier);
			it->second.channel_unset();
		};

	protected:
		void plug_register(types::string parameter_specifier, types::boolean required=values::boolean_false) {
			plugs.insert(std::make_pair(parameter_specifier, plug_configuration()));
		};

		void plug_register(types::string parameter_specifier, types::sample range_min, types::sample range_max, types::boolean required=values::boolean_false) {
			plugs.insert(std::make_pair(parameter_specifier, plug_configuration(range_min, range_max)));
		};

		types::boolean plugged(types::string parameter_specifier) {
			return plugged_currently.find(parameter_specifier) != plugged_currently.end();
		};

		types::channel plug_channel_get(types::string parameter_specifier) {
			auto it = plugs.find(parameter_specifier);
			types::boolean pluggable = it != plugs.end();

#ifdef CDSP_DEBUG_INTERNAL
			cdsp::ensure(pluggable);
#endif

			return it->second.channel_get();
		};


	private:
		class plug_configuration {
		public:
			plug_configuration() :
				range_valid(values::boolean_false),
				channel_valid(values::boolean_false)
			{};

			plug_configuration(types::sample _range_min, types::sample _range_max) :
				range_min(_range_min),
				range_max(_range_max),
				range_valid(values::boolean_true),
				channel_valid(values::boolean_false)
			{};

			types::channel channel_get() {
				return channel;
			};

			void channel_set(types::channel _channel) {
				channel = _channel;
				channel_valid = values::boolean_true;
			};
			
			void channel_unset() {
				channel_valid = values::boolean_false;
			};

		private:
			types::boolean range_valid;
			types::sample range_min;
			types::sample range_max;

			types::boolean channel_valid;
			types::channel channel;
		};

		std::unordered_map<types::string, plug_configuration > plugs;
		std::set<types::string> plugged_currently;
	};
}}

#endif
