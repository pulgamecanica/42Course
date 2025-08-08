// access_tests.hpp

#pragma once
#include "test_framework.hpp"
#include <stdexcept>

namespace access {

template <typename C>
TestList<C> get_table() {
	return {
		{
			"front()",
			[](C& a, C&) {
				if (!a.empty())
					(void)a.front();
			}
		},
		{
			"back()",
			[](C& a, C&) {
				if (!a.empty())
					(void)a.back();
			}
		}
	};
}

} // namespace access
