#pragma once
#include "test_framework.hpp"

namespace sequence_modifiers {

template <typename C>
TestList<C> get_table() {
	return {
		{
			"insert(pos, value)",
			[](C& a, C&) {
				a.insert(a.begin(), typename C::value_type());
			}
		},
		{
			"insert(pos, count, value)",
			[](C& a, C&) {
				a.insert(a.begin(), 2, typename C::value_type());
			}
		},
		{
			"insert(pos, range)",
			[](C& a, C&) {
				C temp;
				temp.push_back(typename C::value_type());
				temp.push_back(typename C::value_type());
				a.insert(a.begin(), temp.begin(), temp.end());
			}
		},
		{
			"erase(pos)",
			[](C& a, C&) {
				if (!a.empty())
					a.erase(a.begin());
			}
		},
		{
			"erase(range)",
			[](C& a, C&) {
				if (a.size() >= 2) {
					typename C::iterator first = a.begin();
					typename C::iterator last = a.begin();
					std::advance(last, 2);
					a.erase(first, last);
				}
			}
		}
	};
}

} // namespace sequence_modifiers

namespace common_modifiers {

template <typename C>
TestList<C> get_table() {
	return {
		{
			"clear()",
			[](C& a, C&) {
				a.clear();
			}
		},
		{
			"swap(a, b)",
			[](C& a, C& b) {
				a.swap(b);
			}
		}
	};
}

} // namespace common_modifiers


namespace associative_modifiers {

template <typename C>
TestList<C> get_table() {
	return {
		{
			"insert(value)",
			[](C& a, C&) {
				typename C::value_type val;
				a.insert(val);
			}
		},
		{
			"erase(key)",
			[](C& a, C&) {
				if (!a.empty()) {
					a.erase(a.begin()->first);
				}
			}
		},
		{
			"erase(iterator)",
			[](C& a, C&) {
				if (!a.empty())
					a.erase(a.begin());
			}
		}
	};
}

} // namespace associative_modifiers
