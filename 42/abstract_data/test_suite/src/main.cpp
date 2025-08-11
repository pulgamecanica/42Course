// src/main.cpp
#include <stdlib.h> // For atoi()

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
	#include "set.hpp"
	#include "vector.hpp"
	#include "deque.hpp"
	#include "list.hpp"
	#include "stack.hpp"
	#include "queue.hpp"
	namespace ns = ft;
#else
	#include <map>
	#include <set>
	#include <vector>
	#include <deque>
	#include <list>
	#include <stack>
	#include <queue>
	namespace ns = std;
#endif

int ntests = 1;

static ns::pair<const int, int> make_pair_int_int() {
	return ns::make_pair(rand() % 100, rand() % 100);
}

static int make_int() {
	return rand() % 100;
}

static void test_vector() {
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
		ntests
	);
}

static void test_deque() {
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
		ntests
	);
}

static void test_list() {
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
		ntests
	);
}

static void test_stack() {
	using Stack = ns::stack<int>;

	test_container<Stack>(
    "STACK",
		container::get_table<Stack>() +
		capacity::simple_get_table<Stack>() +
		sequence::get_stack_table<Stack>() +
		access::get_stack_table<Stack>() +
		relational::get_table<Stack>(),
		ntests
	);
}

static void test_map() {
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
		ntests
	);
}

static void test_multimap() {
	using MultiMap = ns::multimap<int, int>;

	test_container<MultiMap>(
    "MULTIMAP",
		container::get_table<MultiMap>() +
		capacity::get_table<MultiMap>() +
		common_modifiers::get_table<MultiMap>() +
		associative::get_table<MultiMap>() +
    associative_modifiers::get_table<MultiMap>(make_pair_int_int) +
		iterators::get_table<MultiMap>() +
		relational::get_table<MultiMap>(),
		ntests
	);
}

static void test_set() {
	using Set = ns::set<int>;

	test_container<Set>(
    "SET",
		container::get_table<Set>() +
		capacity::get_table<Set>() +
		common_modifiers::get_table<Set>() +
    associative_modifiers::get_table<Set>(make_int) +
		associative::get_table<Set>() +
		iterators::get_table<Set>() +
		relational::get_table<Set>(),
		ntests
	);
}

static void test_multiset() {
	using MultiSet = ns::multiset<int>;

	test_container<MultiSet>(
    "MULTISET",
		container::get_table<MultiSet>() +
		capacity::get_table<MultiSet>() +
		common_modifiers::get_table<MultiSet>() +
    associative_modifiers::get_table<MultiSet>(make_int) +
		associative::get_table<MultiSet>() +
		iterators::get_table<MultiSet>() +
		relational::get_table<MultiSet>(),
		ntests
	);
}

static void test_queue() {
	using Queue = ns::queue<int>;

	test_container<Queue>(
    "QUEUE",
		container::get_table<Queue>() +
		capacity::simple_get_table<Queue>() +
		sequence::get_stack_table<Queue>() +
		access::get_table<Queue>() +
		relational::get_table<Queue>(),
		ntests
	);
}

static void test_priority_queue() {
	using Queue = ns::priority_queue<int>;

	test_container<Queue>(
    "PRIORITY_QUEUE",
		container::get_table<Queue>() +
		capacity::simple_get_table<Queue>() +
		sequence::get_stack_table<Queue>(),
		ntests
	);
}

int main(int ac, char *av[]) {
	std::srand(std::time(nullptr));

	if (ac > 1) {
		ntests = atoi(av[1]);
	}
	
	if (ntests <= 0) {
		std::cout << "Number of tests should be > 0 (got: " << av[1] << ")" << std::endl;
		return 1;
	}
	test_vector();
	test_list();
	test_stack();
	test_deque();
  test_map();
  test_set();
	test_multimap();
	test_multiset();
	test_queue();
	test_priority_queue();
	return 0;
}

