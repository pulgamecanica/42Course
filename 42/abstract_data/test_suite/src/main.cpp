// src/main.cpp

#include "test_framework.hpp"
#include "container_tests.hpp"
#include "sequence_tests.hpp"
#include "capacity_tests.hpp"
#include "modifiers_tests.hpp"
#include "access_tests.hpp"
#include "iterator_tests.hpp"
#include "relational_tests.hpp"
#include "assign_tests.hpp"

#ifdef MODE_FT
	#include "vector.hpp"
	#include "deque.hpp"
	#include "list.hpp"
	namespace ns = ft;
#else
	#include <vector>
	#include <deque>
	#include <list>
	namespace ns = std;
#endif

static void test_vector() {
  int ntest = 1;

	using Vec = ns::vector<int>;

	test_container<Vec>(
    "VECTOR",
		container::get_table<Vec>() +
		full_capacity::get_table<Vec>() +
		sequence::get_table<Vec>() +
		common_modifiers::get_table<Vec>()  +
		sequence_modifiers::get_table<Vec>()  +
    access::get_table<Vec>() +
    assign::get_table<Vec>() +
    iterators::get_table<Vec>() +
    relational::get_table<Vec>(),
		ntest
	);
}

static void test_deque() {
	int ntest = 1;
	using Deq = ns::deque<int>;

	test_container<Deq>(
    "DEQUE",
		container::get_table<Deq>() +
		capacity::get_table<Deq>() +
		sequence::get_table<Deq>() + // includes operator[], at(), etc.
		common_modifiers::get_table<Deq>() +
		sequence_modifiers::get_table<Deq>() +
		assign::get_table<Deq>() +
		access::get_table<Deq>() +
		iterators::get_table<Deq>() +
		relational::get_table<Deq>(),
		ntest
	);
}

static void test_list() {
	int ntest = 1;
	using List = ns::list<int>;

	test_container<List>(
    "LIST",
		container::get_table<List>() +
		capacity::get_table<List>() +
		common_modifiers::get_table<List>() +
		sequence_modifiers::get_table<List>() +
		assign::get_table<List>() +
		access::get_table<List>() +
		iterators::get_table<List>() +
		relational::get_table<List>(),
		ntest
	);
}


int main() {
	std::srand(std::time(nullptr));

	test_vector();
	test_list();
	test_deque();

	return 0;
}

