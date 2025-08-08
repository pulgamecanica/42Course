// assign_tests.hpp

#pragma once
#include "test_framework.hpp"

namespace assign {

template <typename C>
TestList<C> get_table() {
	return {
		{
			"assign(count, value)",
			[](C& a, C&) {
				a.assign(5, typename C::value_type());
			}
		},
		{
			"assign(first, last)",
			[](C& a, C&) {
				C temp;
				temp.push_back(typename C::value_type());
				temp.push_back(typename C::value_type());
				a.assign(temp.begin(), temp.end());
			}
		}
	};
}

} // namespace assign
