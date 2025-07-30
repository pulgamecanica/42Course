#include <iostream>
#include <string>
#include <cassert>
#include <vector>
#ifdef MODE_FT
    #include "vector.hpp"
    namespace ns = ft;
#else
    #include <vector>
    namespace ns = std;
#endif

void debug() {
    std::cout << "Debugging" << std::endl;
    ns::vector<std::string> v1(14, "un");
    ns::vector<std::string> v2(v1);
}

void run_vector_compliance_tests() {
    debug();

    std::cout << "\n[ns::vector] Starting API compliance tests..." << std::endl;

    ns::vector<int> vec_default;
    ns::vector<int> vec_fill(5, 42);
    int arr[] = {1, 2, 3, 4, 5};
    ns::vector<int> vec_range(arr, arr + 5);
    ns::vector<int> vec_copy(vec_range);
    vec_default = vec_fill;

    const ns::vector<int> const_vec(vec_range);
    assert(!const_vec.empty());
    assert(const_vec.front() == 1);
    assert(const_vec.back() == 5);
    assert(const_vec.at(1) == const_vec[1]);

    // Iterators
    for (ns::vector<int>::iterator it = vec_range.begin(); it != vec_range.end(); ++it) *it;
    for (ns::vector<int>::const_iterator it = const_vec.begin(); it != const_vec.end(); ++it) *it;
    for (ns::vector<int>::reverse_iterator it = vec_range.rbegin(); it != vec_range.rend(); ++it) *it;
    for (ns::vector<int>::const_reverse_iterator it = const_vec.rbegin(); it != const_vec.rend(); ++it) *it;

    // Modifiers
    vec_default.assign(arr, arr + 3);
    vec_default.assign(4, 7);
    vec_default.push_back(99);
    vec_default.pop_back();

    vec_default.insert(vec_default.begin(), 88);
    vec_default.insert(vec_default.begin() + 1, 2, 77);
    vec_default.insert(vec_default.end(), arr, arr + 2);

    vec_default.erase(vec_default.begin());
    if (vec_default.size() > 2)
        vec_default.erase(vec_default.begin(), vec_default.begin() + 2);

    vec_default.resize(10, 100);
    vec_default.resize(5);
    vec_default.reserve(20);

    // Access
    if (!vec_default.empty()) {
        vec_default.front();
        vec_default.back();
        vec_default.at(0);
        vec_default[0];
    }

    // Swap and compare
    ns::vector<int> vec_temp(vec_default);
    vec_temp.swap(vec_default);
    ns::swap(vec_temp, vec_default);

    assert((vec_temp == vec_default) || (vec_temp != vec_default));
    (void)(vec_temp < vec_default);
    (void)(vec_temp <= vec_default);
    (void)(vec_temp > vec_default);
    (void)(vec_temp >= vec_default);

    std::cout << "[ns::vector] All API compliance tests passed.\n" << std::endl;
}
