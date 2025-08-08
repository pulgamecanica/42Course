// relational_tests.hpp

#pragma once
#include "test_framework.hpp"

namespace relational {

template <typename C>
TestList<C> get_table() {
	return {
		{
			"operator==",
			[](C& a, C& b) {
				(void)(a == b);
			}
		},
		{
			"operator!=",
			[](C& a, C& b) {
				(void)(a != b);
			}
		},
		{
			"operator<",
			[](C& a, C& b) {
				(void)(a < b);
			}
		},
		{
			"operator>",
			[](C& a, C& b) {
				(void)(a > b);
			}
		},
		{
			"operator<=",
			[](C& a, C& b) {
				(void)(a <= b);
			}
		},
		{
			"operator>=",
			[](C& a, C& b) {
				(void)(a >= b);
			}
		}
	};
}

} // namespace relational
