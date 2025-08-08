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
#include "associative_tests.hpp"
#include "map_extra_tests.hpp"

#ifdef MODE_FT
	#include "map.hpp"
	#include "vector.hpp"
	#include "deque.hpp"
	#include "list.hpp"
	namespace ns = ft;
#else
	#include <map>
	#include <vector>
	#include <deque>
	#include <list>
	namespace ns = std;
#endif

static ns::pair<const int, int> make_pair_int_int() {
	return ns::make_pair(rand() % 100, rand() % 100);
}

// static int make_int() {
// 	return rand() % 100;
// }

// static ns::pair<std::string, double> make_string_double() {
// 	static int counter = 0;
// 	return ns::make_pair("key" + std::to_string(counter++), rand() % 1000 / 10.0);
// }


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

static void test_map() {
	int ntest = 1;
	using Map = ns::map<int, int>;

	test_container<Map>(
    "MAP",
		container::get_table<Map>() +
		capacity::get_table<Map>() +
		common_modifiers::get_table<Map>() +
		associative::get_table<Map>() +
    associative_modifiers::get_table<Map>(make_pair_int_int) +
		map_extras::get_table<Map>() +
		iterators::get_table<Map>() +
		relational::get_table<Map>(),
		ntest
	);
}


int main() {
	std::srand(std::time(nullptr));

	test_vector();
	test_list();
	test_deque();
  test_map();

	return 0;
}

