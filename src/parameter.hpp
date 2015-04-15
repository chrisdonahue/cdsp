#ifndef CDSP_PARAMETER
#define CDSP_PARAMETER

#include <limits>
#include <unordered_map>

#include "dsp.hpp"
#include "exception.hpp"
#include "helper.hpp"
#include "types.hpp"
#include "values.hpp"

namespace cdsp { namespace parameter {
	template <typename T>
	class base {
	public:
		base() :
			value_initial_valid(values::boolean_false),
			value_valid(values::boolean_false),
			value_min_valid(values::boolean_false),
			value_max_valid(values::boolean_false)
		{};

		base(T _value) :
			value_initial(_value),
			value(_value),
			value_initial_valid(values::boolean_true),
			value_valid(values::boolean_true),
			value_min_valid(values::boolean_false),
			value_max_valid(values::boolean_false)
		{};

		base(T _value_min, T _value_max) :
			value_min(_value_min),
			value_max(_value_max),
			value_initial_valid(values::boolean_false),
			value_valid(values::boolean_false),
			value_min_valid(values::boolean_true),
			value_max_valid(values::boolean_true)
		{};

		base(T _value, T _value_min, T _value_max) :
			value_initial(_value),
			value(_value),
			value_min(_value_min),
			value_max(_value_max),
			value_initial_valid(values::boolean_true),
			value_valid(values::boolean_true),
			value_min_valid(values::boolean_true),
			value_max_valid(values::boolean_true)
		{};

		void reset() {
#ifdef CDSP_DEBUG_API
			if (!value_initial_valid) {
				throw exception::runtime("tried to reset parameter before specifying an initial value");
			}
#endif

			value = value_initial;
		};

		T value_initial_get() {
#ifdef CDSP_DEBUG_API
			if (!value_initial_valid) {
				throw exception::runtime("tried to retrieve initial value before setting it");
			}
#endif

			return value_initial;
		};

		void value_initial_set(T _value_initial) {
			value_initial = _value_initial;
			value_initial_set = values::boolean_true;
		};

		T value_get() {
#ifdef CDSP_DEBUG_API
			if (!value_valid) {
				throw exception::runtime("tried to retrieve value before setting it");
			}
#endif

			return value;
		};

		void value_set(T _value) {
			value = _value;
			value_valid = values::boolean_true;
			if (!value_initial_valid) {
				value_initial = value;
				value_initial_valid = values::boolean_true;
			}
		};

		types::boolean value_range_valid_get() {
			return value_min_valid && value_max_valid;
		};

		T value_min_get() {
#ifdef CDSP_DEBUG_API
			if (!value_min_valid) {
				// TODO: throw exception
			}
#endif

			return value_min;
		};

		void value_min_set(T _value_min) {
			value_min = _value_min;
			value_min_valid = true;
		};

		T value_max_get() {
#ifdef CDSP_DEBUG_API
			if (!value_max_valid) {
				// TODO: throw exception
			}
#endif

			return value_max;
		};

		void value_max_set(T _value_max) {
			value_max = _value_max;
			value_max_valid = true;
		};

		void value_range_set(T _value_min, T _value_max) {
			value_min = _value_min;
			value_min_valid = true;
			value_max = _value_max;
			value_max_valid = true;
		};

	protected:
		T value_initial;
		T value;
		T value_min;
		T value_max;

	private:
		types::boolean value_initial_valid;
		types::boolean value_valid;
		types::boolean value_min_valid;
		types::boolean value_max_valid;
	};
}}

#endif
