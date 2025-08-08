// capacity_tests.hpp

#pragma once
#include "test_framework.hpp"
#include <stdexcept>
#include <limits>

namespace capacity {

template <typename C>
TestList<C> get_table() {
	return {
		{
			"Check empty() and size()",
			[](C& a, C&) {
				bool e = a.empty();
				typename C::size_type s = a.size();
				if (e && s != 0)
					throw std::runtime_error("empty() is true but size() != 0");
				if (!e && s == 0)
					throw std::runtime_error("empty() is false but size() == 0");
			}
		},
		{
			"Check max_size() > 0",
			[](C& a, C&) {
				if (a.max_size() == 0)
					throw std::runtime_error("max_size() == 0");
			}
		}
	};
}

} // namespace capacity

namespace full_capacity {

template <typename C>
TestList<C> get_table() {
	TestList<C> out = capacity::get_table<C>();

	out.push_back(
		{
			"capacity() check",
			[](C& a, C&) {
				(void)a.capacity();
			}
		}
	);
	
	out.push_back(
		{
			"reserve(n)",
			[](C& a, C&) {
				std::size_t new_cap = a.capacity() + 10;
				a.reserve(new_cap);
				if (a.capacity() < new_cap)
					throw std::runtime_error("reserve() failed to grow capacity");
			}
		}
	);

	return out;
}

} // namespace capacity
