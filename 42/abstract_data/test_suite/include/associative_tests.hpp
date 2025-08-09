// associative_tests.hpp

#pragma once
#include "test_framework.hpp"
#include <stdexcept>

#ifdef MODE_FT
  #include "utility.hpp"
#else
  #include <utility>
#endif

namespace associative {

template <typename C>
TestList<C> get_table() {
	return {
		{
			"insert(value)",
			[](C& a, C&) {
				typename C::value_type v{};
				a.insert(v); // if multimap or map, v = pair<const K, T>
			}
		},
		{
			"erase(key)",
			[](C& a, C&) {
				if (!a.empty())
				  (void)a.erase(typename C::key_type());
			}
		},
		{
			"erase(iterator)",
			[](C& a, C&) {
				if (!a.empty())
					a.erase(a.begin());
			}
		},
		{
			"clear()",
			[](C& a, C&) {
				a.clear();
			}
		},
		{
			"find(key)",
			[](C& a, C&) {
				(void)a.find(typename C::key_type());
			}
		},
		{
			"count(key)",
			[](C& a, C&) {
				(void)a.count(typename C::key_type());
			}
		},
		{
			"equal_range(key)",
			[](C& a, C&) {
				(void)a.equal_range(typename C::key_type());
			}
		}
	};
}

} // namespace associative
