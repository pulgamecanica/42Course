// sequence_tests.hpp

#pragma once
#include "test_framework.hpp"

namespace sequence {

template <typename C>
TestList<C> get_table() {
	return {
		{
			"push_back(value)",
			[](C& a, C&) {
				a.push_back(rand() % 100);
			}
		},
		{
			"pop_back()",
			[](C& a, C&) {
				if (!a.empty())
					a.pop_back();
			}
		},
		{
			"resize(n)",
			[](C& a, C&) {
				std::size_t new_size = rand() % 20;
				a.resize(new_size);
			}
		},
		{
			"at(i)",
			[](C& a, C&) {
				if (!a.empty()) {
					std::size_t i = rand() % a.size();
					a.at(i);
				}
			}
		},
		{
			"operator[](i)",
			[](C& a, C&) {
				if (!a.empty()) {
					std::size_t i = rand() % a.size();
					(void)a[i];
				}
			}
		}
	};
}

} // namespace sequence
