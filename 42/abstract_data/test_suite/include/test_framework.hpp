// test_framework.hpp

#pragma once
#include <vector>
#include <string>
#include <functional>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <utility>

template <typename C>
using TestFn = std::function<void(C&, C&)>;

template <typename C>
using TestList = std::vector<std::pair<std::string, TestFn<C>>>;

template <typename C>
TestList<C> operator+(const TestList<C>& a, const TestList<C>& b) {
	TestList<C> out = a;
	out.insert(out.end(), b.begin(), b.end());
	return out;
}

template <typename C>
void print(const C& container) {
	std::cout << "[";
	typename C::const_iterator it = container.begin();
	typename C::const_iterator end = container.end();
	while (it != end) {
		// Special handling for containers of pairs (e.g. map)
		std::cout << *it;
		++it;
		if (it != end)
			std::cout << ", ";
	}
	std::cout << "]\n";
}

// Overload for containers of std::pair (e.g. map)
template <typename K, typename V>
std::ostream& operator<<(std::ostream& os, const std::pair<K, V>& p) {
	return os << "{" << p.first << ": " << p.second << "}";
}

template <typename C>
void test_container(const std::string& container_name, const TestList<C>& tests, size_t count_per_test = 1) {
	if (tests.empty()) return;

	C c1;
	C c2;
	C* a = &c1;
	C* b = &c2;

	size_t test_number = 0;

	for (const auto& [name, fn] : tests) {
		if (rand() % 2) std::swap(a, b);
		for (size_t i = 0; i < count_per_test; ++i) {

#ifdef DEBUG
			std::cout << "<========================START==========================>\n";
			std::cout << "Before:\n";
			std::cout << "  a: ";
			print(*a);
			std::cout << "  b: ";
			print(*b);
#endif

			std::cout << "\n[ |" << container_name << "| Test " << (++test_number) << " ] " << name << " (run " << i+1 << ")\n";

			try {
				fn(*b, *a);
				std::cout << "✓ OK\n";
			} catch (const std::exception& e) {
				std::cout << "✗ FAILED: " << e.what() << "\n";
			} catch (...) {
				std::cout << "✗ FAILED: unknown error\n";
			}

#ifdef DEBUG
			std::cout << "\nAfter:\n";
			std::cout << "  a: ";
			print(*a);
			std::cout << "  b: ";
			print(*b);
			std::cout << "<=========================END===========================>\n\n";
#endif

		}
	}
}
