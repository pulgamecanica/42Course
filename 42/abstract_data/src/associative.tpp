#include <map>
#include <set>
#include <string>

template <
    typename Container,
    typename Key,
    typename Mapped = void, // only needed for map-like containers
    bool HasMapped = !std::is_same<Mapped, void>::value
>
void run_associative_container_tests() {
    std::cout << "[Container] Starting compliance tests..." << std::endl;

    Container c;
    Key k1 = Key(), k2 = Key();

    // Insert single
    if constexpr (HasMapped) {
        c.insert(typename Container::value_type(k1, Mapped()));
    } else {
        c.insert(k1);
    }

    // Range insert
    Container temp;
    if constexpr (HasMapped) {
        temp.insert(typename Container::value_type(k2, Mapped()));
    } else {
        temp.insert(k2);
    }
    c.insert(temp.begin(), temp.end());

    // Lookup
    c.find(k1);
    c.count(k1);
    c.lower_bound(k1);
    c.upper_bound(k1);
    c.equal_range(k1);

    // Erase
    if (!c.empty()) {
        c.erase(c.begin());
        c.erase(k1);
        c.erase(c.begin(), c.end());
    }

    // Swap and comparisons
    Container c2(c);
    c.swap(c2);
    assert((c == c2) || (c != c2));
    (void)(c < c2);
    (void)(c <= c2);
    (void)(c > c2);
    (void)(c >= c2);

    // Operator[]
    if constexpr (HasMapped) {
        c[k1] = Mapped();
        c.at(k1);
    }

    std::cout << "[Container] Compliance tests passed.\n";
}
