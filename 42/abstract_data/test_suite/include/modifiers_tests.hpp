#pragma once
#include "test_framework.hpp"

#ifdef MODE_FT
	#include "map.hpp"
	#include "set.hpp"
	#include "utility.hpp"

	namespace ns = ft;
#else
	#include <map>
	#include <set>
	#include <utility>
	namespace ns = std;
#endif

template <typename Value>
struct key_of {
	using type = Value;
};

template <typename Key, typename T>
struct key_of<ns::pair<const Key, T>> {
	using type = Key;
};

template <typename T>
T extract_key(const T& x) {
	return x;
}

template <typename K, typename V>
K extract_key(const ns::pair<const K, V>& x) {
	return x.first;
}

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
TestList<C> get_table(typename C::value_type (*make_value)()) {
	TestList<C> out;

	out.push_back({
		"insert(value)",
		[make_value](C& a, C&) {
			a.insert(make_value());
		}
	});

	out.push_back({
		"insert(hint, value)",
		[make_value](C& a, C&) {
			a.insert(a.begin(), make_value());
		}
	});

	out.push_back({
		"insert(range)",
		[make_value](C& a, C&) {
			C tmp;
			tmp.insert(make_value());
			tmp.insert(make_value());
			a.insert(tmp.begin(), tmp.end());
		}
	});

	out.push_back({
		"erase(iterator)",
		[](C& a, C&) {
			if (!a.empty())
				a.erase(a.begin());
		}
	});

	out.push_back({
		"erase(key)",
		[](C& a, C&) {
			if (!a.empty()) {
				typename key_of<typename C::value_type>::type key = extract_key(*a.begin());
				a.erase(key);
			}
		}
	});

	out.push_back({
		"erase(range)",
		[](C& a, C&) {
			if (a.size() >= 2) {
				typename C::iterator first = a.begin();
				typename C::iterator last = a.begin();
				std::advance(last, 2);
				a.erase(first, last);
			}
		}
	});

	return out;
}

} // namespace associative_modifiers
