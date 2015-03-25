#ifndef CDSP_CIRCULAR_BUFFER
#define CDSP_CIRCULAR_BUFFER

#include "types.hpp"

namespace cdsp {
	template <typename T>
	class circular_buffer<T> {
	public:
		circular_buffer();
		circular_buffer(types::index items_num);
		circular_buffer(types::index items_num, const T* items);
		circular_buffer(std::vector<T> items);

		void resize(types::index items_num_new);

		void add(T item) {
		};
		void remove(types::index);
	private:
		types::index items_num;
		T* head;
		T* buffer;
	};
}

#endif