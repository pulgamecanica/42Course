// map_tests.hpp

#pragma once
#include "test_framework.hpp"

namespace map_extras {

template <typename C>
TestList<C> get_table() {
	return {
		{
			"operator[] access and insert",
			[](C& a, C&) {
				a[rand() % 100] = typename C::mapped_type();
			}
		}
	};
}

} // namespace map_extras
