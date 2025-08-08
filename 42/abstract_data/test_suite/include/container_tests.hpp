// container_tests.hpp

#pragma once
#include "test_framework.hpp"

namespace container {

template <typename C>
TestList<C> get_table() {
	return {
		{
			"Default constructor",
			[](C& a, C&) {
				C temp;
				a = temp;
			}
		},
		{
			"Copy constructor",
			[](C& a, C&) {
				C copy(a);
				(void)copy;
			}
		},
		{
			"Assignment operator",
			[](C& a, C& b) {
				a = b;
			}
		},
		{
			"Clear",
			[](C& a, C&) {
				a.clear();
			}
		}
	};
}

} // namespace container
