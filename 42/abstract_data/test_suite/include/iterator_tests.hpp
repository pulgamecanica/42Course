// iterator_tests.hpp

#pragma once
#include "test_framework.hpp"
#include <iterator>

namespace iterators {

template <typename C>
TestList<C> get_table() {
	return {
		{
			"begin/end traversal",
			[](C& a, C&) {
				for (typename C::iterator it = a.begin(); it != a.end(); ++it)
					(void)*it;
			}
		},
		{
			"const begin/end",
			[](C& a, C&) {
				const C& ca = a;
				for (typename C::const_iterator it = ca.begin(); it != ca.end(); ++it)
					(void)*it;
			}
		},
		{
			"rbegin/rend traversal",
			[](C& a, C&) {
				for (typename C::reverse_iterator it = a.rbegin(); it != a.rend(); ++it)
					(void)*it;
			}
		}
	};
}

} // namespace iterators
