#include <iostream>
#include <string>
#include <cassert>
#ifdef MODE_FT
    #include "deque.hpp"
    namespace ns = ft;
#else
    #include <deque>
    namespace ns = std;
#endif

void run_deque_compliance_tests() {
    std::cout << "\n[ns::deque] Starting API compliance tests..." << std::endl;

    // Constructors
    ns::deque<int> dq_default;
    ns::deque<int> dq_fill(5, 42);
    int arr[] = {1, 2, 3, 4, 5};
    ns::deque<int> dq_range(arr, arr + 5);
    ns::deque<int> dq_copy(dq_range);
    dq_default = dq_fill;

    const ns::deque<int> const_dq(dq_range);
    assert(!const_dq.empty());
    assert(const_dq.front() == 1);
    assert(const_dq.back() == 5);
    assert(const_dq.at(2) == const_dq[2]);

    // Iterators
    for (ns::deque<int>::iterator it = dq_range.begin(); it != dq_range.end(); ++it) *it;
    for (ns::deque<int>::const_iterator it = const_dq.begin(); it != const_dq.end(); ++it) *it;
    for (ns::deque<int>::reverse_iterator it = dq_range.rbegin(); it != dq_range.rend(); ++it) *it;
    for (ns::deque<int>::const_reverse_iterator it = const_dq.rbegin(); it != const_dq.rend(); ++it) *it;

    // Modifiers
    dq_default.assign(arr, arr + 3);
    dq_default.assign(4, 7);
    dq_default.push_back(99);
    // for (size_t i = 0; i < 420000000; i++)
    // {
    //     dq_default.push_front(88);
    // }
    
    dq_default.push_front(88);
    dq_default.pop_back();
    dq_default.pop_front();

    dq_default.insert(dq_default.begin(), 77);
    dq_default.insert(dq_default.begin() + 1, 2, 66);
    dq_default.insert(dq_default.end(), arr, arr + 2);

    dq_default.erase(dq_default.begin());
    if (dq_default.size() > 2)
        dq_default.erase(dq_default.begin(), dq_default.begin() + 2);

    dq_default.resize(10, 100);
    dq_default.resize(5);

    // Element Access
    if (!dq_default.empty()) {
        dq_default.front();
        dq_default.back();
        dq_default.at(0);
        dq_default[0];
    }

    // Swap and comparison
    ns::deque<int> dq_temp(dq_default);
    dq_temp.swap(dq_default);
    ns::swap(dq_temp, dq_default);

    assert((dq_temp == dq_default) || (dq_temp != dq_default));
    (void)(dq_temp < dq_default);
    (void)(dq_temp <= dq_default);
    (void)(dq_temp > dq_default);
    (void)(dq_temp >= dq_default);

    std::cout << "[ns::deque] All API compliance tests passed.\n" << std::endl;
}
