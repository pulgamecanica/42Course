  #include <iostream>
  #include <fstream>
  #include <vector>
  #include <deque>
  #include <list>
  #include <map>
  #include <set>
  #include <string>
  #include <ctime>
  #include <cstdlib>
  #include <functional>
  #include "vector.hpp"
  #include "deque.hpp"
  #include "list.hpp"
  #include "map.hpp"
  #include "set.hpp"
  #include "benchmark.hpp"

namespace benchmark {

  // Benchmark all C++98 <ns::list> member functions (timing only, no return validation)
template <typename List, typename T>
void benchmark_cpp98_list(const std::string& ns_label, const std::string& type_label, std::size_t count, std::ofstream& out) {
  std::vector<T> data = generate_data<T>(count);
  std::string prefix = "list," + type_label;

  auto bench = [&](const std::string& name, const std::function<void()>& func) {
    double t = measure_time(func);
    out << prefix << "," << name << "," << count << "," << ns_label << "," << t << "\n";
  };

  bench("constructor_default", std::function<void()>([&]() { List l; }));
  bench("constructor_fill", std::function<void()>([&]() { List l(count, T()); }));
  bench("constructor_range", std::function<void()>([&]() { List l(data.begin(), data.end()); }));
  bench("constructor_copy", std::function<void()>([&]() { List l1(data.begin(), data.end()); List l2(l1); }));
  bench("operator_assign", std::function<void()>([&]() { List l1(data.begin(), data.end()); List l2; l2 = l1; }));

  bench("assign_range", std::function<void()>([&]() { List l; l.assign(data.begin(), data.end()); }));
  bench("assign_fill", std::function<void()>([&]() { List l; l.assign(count, T()); }));

  bench("get_allocator", std::function<void()>([&]() { List l; typename List::allocator_type a = l.get_allocator(); (void)a; }));

  bench("begin_end_iter", std::function<void()>([&]() {
    List l(data.begin(), data.end());
    for (typename List::iterator it = l.begin(); it != l.end(); ++it) { volatile T x = *it; (void)x; }
  }));
  bench("rbegin_rend_iter", std::function<void()>([&]() {
    List l(data.begin(), data.end());
    for (typename List::reverse_iterator it = l.rbegin(); it != l.rend(); ++it) { volatile T x = *it; (void)x; }
  }));

  bench("size", std::function<void()>([&]() { List l(data.begin(), data.end()); volatile std::size_t s = l.size(); (void)s; }));
  bench("max_size", std::function<void()>([&]() { List l; volatile std::size_t m = l.max_size(); (void)m; }));
  bench("resize_down", std::function<void()>([&]() { List l(data.begin(), data.end()); l.resize(count / 2); }));
  bench("resize_up", std::function<void()>([&]() { List l(data.begin(), data.end()); l.resize(count * 2, T()); }));
  bench("empty", std::function<void()>([&]() { List l; volatile bool e = l.empty(); (void)e; }));

  bench("front_back", std::function<void()>([&]() {
    List l(data.begin(), data.end());
    volatile T f = l.front(); (void)f;
    volatile T b = l.back(); (void)b;
  }));

  bench("push_back", std::function<void()>([&]() {
    List l;
    for (std::size_t i = 0; i < count; ++i) l.push_back(data[i]);
  }));
  bench("push_front", std::function<void()>([&]() {
    List l;
    for (std::size_t i = 0; i < count; ++i) l.push_front(data[i]);
  }));

  bench("pop_back", std::function<void()>([&]() {
    List l(data.begin(), data.end());
    for (std::size_t i = 0; i < count; ++i) l.pop_back();
  }));
  bench("pop_front", std::function<void()>([&]() {
    List l(data.begin(), data.end());
    for (std::size_t i = 0; i < count; ++i) l.pop_front();
  }));

  bench("insert_single", std::function<void()>([&]() {
    List l(data.begin(), data.end());
    l.insert(++l.begin(), T());
  }));
  bench("insert_fill", std::function<void()>([&]() {
    List l(data.begin(), data.end());
    l.insert(++l.begin(), 10, T());
  }));
  bench("insert_range", std::function<void()>([&]() {
    List l(data.begin(), data.end());
    l.insert(++l.begin(), data.begin(), data.begin() + std::min(count, std::size_t(10)));
  }));

  bench("erase_single", std::function<void()>([&]() {
  List l(data.begin(), data.end());
  l.erase(++l.begin());
  }));
  bench("erase_range", std::function<void()>([&]() {
    List l(data.begin(), data.end());
    typename List::iterator first = l.begin(), last = l.begin();
    std::advance(first, count / 4);
    std::advance(last, count / 2);
    l.erase(first, last);
  }));

  bench("swap", std::function<void()>([&]() {
    List l1(data.begin(), data.end());
    List l2(count, T());
    l1.swap(l2);
  }));

  bench("clear", std::function<void()>([&]() {
    List l(data.begin(), data.end());
    l.clear();
  }));

  // list-specific operations
  bench("splice_all", std::function<void()>([&]() {
    List l1(data.begin(), data.end());
    List l2;
    l2.splice(l2.begin(), l1);
  }));
  bench("splice_one", std::function<void()>([&]() {
    List l1(data.begin(), data.end());
    List l2;
    typename List::iterator it = l1.begin();
    l2.splice(l2.begin(), l1, it);
  }));
  bench("splice_range", std::function<void()>([&]() {
    List l1(data.begin(), data.end());
    List l2;
    typename List::iterator first = l1.begin(), last = l1.begin();
    std::advance(last, std::min(count, std::size_t(10)));
    l2.splice(l2.begin(), l1, first, last);
  }));

  bench("remove_value", std::function<void()>([&]() {
    List l(data.begin(), data.end());
    l.remove(data[0]);
  }));
    bench("remove_if", std::function<void()>([&]() {
    List l(data.begin(), data.end());
    l.remove_if([](const T& val) { return val == T(); });
  }));

  bench("unique_default", std::function<void()>([&]() {
    List l(data.begin(), data.end());
    l.unique();
  }));
  bench("unique_pred", std::function<void()>([&]() {
    List l(data.begin(), data.end());
    l.unique([](const T& a, const T& b) { return a == b; });
  }));

  bench("merge_default", std::function<void()>([&]() {
    List l1(data.begin(), data.end());
    List l2(data.begin(), data.end());
    l1.merge(l2);
  }));
  bench("merge_comp", std::function<void()>([&]() {
    List l1(data.begin(), data.end());
    List l2(data.begin(), data.end());
    l1.merge(l2, [](const T& a, const T& b) { return a < b; });
  }));

  bench("sort_default", std::function<void()>([&]() {
    List l(data.begin(), data.end());
    l.sort();
  }));
  bench("sort_comp", std::function<void()>([&]() {
    List l(data.begin(), data.end());
    l.sort([](const T& a, const T& b) { return a < b; });
  }));

  bench("reverse", std::function<void()>([&]() {
    List l(data.begin(), data.end());
    l.reverse();
  }));
}

  // Benchmark all C++98 <ns::deque> member functions (timing only, no return validation)
template <typename Deq, typename T>
void benchmark_cpp98_deque(const std::string& ns_label, const std::string& type_label, std::size_t count, std::ofstream& out) {
  std::vector<T> data = generate_data<T>(count);
  std::string prefix = "deque," + type_label;

  auto bench = [&](const std::string& name, const std::function<void()>& func) {
    double t = measure_time(func);
    out << prefix << "," << name << "," << count << "," << ns_label << "," << t << "\n";
  };

  bench("constructor_default", std::function<void()>([&]() { Deq d; }));
  bench("constructor_fill", std::function<void()>([&]() { Deq d(count, T()); }));
  bench("constructor_range", std::function<void()>([&]() { Deq d(data.begin(), data.end()); }));
  bench("constructor_copy", std::function<void()>([&]() { Deq d1(data.begin(), data.end()); Deq d2(d1); }));
  bench("operator_assign", std::function<void()>([&]() { Deq d1(data.begin(), data.end()); Deq d2; d2 = d1; }));

  bench("assign_range", std::function<void()>([&]() { Deq d; d.assign(data.begin(), data.end()); }));
  bench("assign_fill", std::function<void()>([&]() { Deq d; d.assign(count, T()); }));

  bench("get_allocator", std::function<void()>([&]() { Deq d; typename Deq::allocator_type a = d.get_allocator(); (void)a; }));

  bench("begin_end_iter", std::function<void()>([&]() {
    Deq d(data.begin(), data.end());
    for (typename Deq::iterator it = d.begin(); it != d.end(); ++it) { volatile T x = *it; (void)x; }
  }));
  bench("rbegin_rend_iter", std::function<void()>([&]() {
    Deq d(data.begin(), data.end());
    for (typename Deq::reverse_iterator it = d.rbegin(); it != d.rend(); ++it) { volatile T x = *it; (void)x; }
  }));

  bench("size", std::function<void()>([&]() { Deq d(data.begin(), data.end()); volatile std::size_t s = d.size(); (void)s; }));
  bench("max_size", std::function<void()>([&]() { Deq d; volatile std::size_t m = d.max_size(); (void)m; }));
  bench("resize_down", std::function<void()>([&]() { Deq d(data.begin(), data.end()); d.resize(count / 2); }));
  bench("resize_up", std::function<void()>([&]() { Deq d(data.begin(), data.end()); d.resize(count * 2, T()); }));
  bench("empty", std::function<void()>([&]() { Deq d; volatile bool e = d.empty(); (void)e; }));

  bench("element_access_operator", std::function<void()>([&]() {
    Deq d(data.begin(), data.end());
    for (std::size_t i = 0; i < d.size(); ++i) { volatile T x = d[i]; (void)x; }
  }));
  bench("element_access_at", std::function<void()>([&]() {
    Deq d(data.begin(), data.end());
    for (std::size_t i = 0; i < d.size(); ++i) { volatile T x = d.at(i); (void)x; }
  }));
  bench("element_access_front_back", std::function<void()>([&]() {
    Deq d(data.begin(), data.end());
    volatile T f = d.front(); (void)f;
    volatile T b = d.back(); (void)b;
  }));

  bench("push_back", std::function<void()>([&]() {
    Deq d;
    for (std::size_t i = 0; i < count; ++i) d.push_back(data[i]);
  }));
  bench("push_front", std::function<void()>([&]() {
    Deq d;
    for (std::size_t i = 0; i < count; ++i) d.push_front(data[i]);
  }));

  bench("pop_back", std::function<void()>([&]() {
    Deq d(data.begin(), data.end());
    for (std::size_t i = 0; i < count; ++i) d.pop_back();
  }));
  bench("pop_front", std::function<void()>([&]() {
    Deq d(data.begin(), data.end());
    for (std::size_t i = 0; i < count; ++i) d.pop_front();
  }));

  bench("insert_single", std::function<void()>([&]() {
    Deq d(data.begin(), data.end());
    d.insert(d.begin() + d.size() / 2, T());
  }));
  bench("insert_fill", std::function<void()>([&]() {
    Deq d(data.begin(), data.end());
    d.insert(d.begin() + d.size() / 2, 10, T());
  }));
  bench("insert_range", std::function<void()>([&]() {
    Deq d(data.begin(), data.end());
    d.insert(d.begin() + d.size() / 2, data.begin(), data.begin() + std::min(count, std::size_t(10)));
  }));

  bench("erase_single", std::function<void()>([&]() {
    Deq d(data.begin(), data.end());
    d.erase(d.begin() + d.size() / 2);
  }));
  bench("erase_range", std::function<void()>([&]() {
    Deq d(data.begin(), data.end());
    d.erase(d.begin() + d.size() / 4, d.begin() + d.size() / 2);
  }));

  bench("swap", std::function<void()>([&]() {
    Deq d1(data.begin(), data.end());
    Deq d2(count, T());
    d1.swap(d2);
  }));
  bench("clear", std::function<void()>([&]() {
    Deq d(data.begin(), data.end());
    d.clear();
  }));
}

  // Benchmark all C++98 member functions (timing only, no return validation)
template <typename Vec, typename T>
void benchmark_cpp98_vector(const std::string& ns_label, const std::string& type_label, std::size_t count, std::ofstream& out) {
  std::vector<T> data = generate_data<T>(count);
  std::string prefix = "vector," + type_label;

  auto bench = [&](const std::string& name, const std::function<void()>& func) {
    double t = measure_time(func);
    out << prefix << "," << name << "," << count << "," << ns_label << "," << t << "\n";
  };

  // Construct/copy/assign
  bench("constructor_default", std::function<void()>([&]() { Vec v; }));
  bench("constructor_fill", std::function<void()>([&]() { Vec v(count, T()); }));
  bench("constructor_range", std::function<void()>([&]() { Vec v(data.begin(), data.end()); }));
  bench("constructor_copy", std::function<void()>([&]() { Vec v1(data.begin(), data.end()); Vec v2(v1); }));
  bench("operator_assign", std::function<void()>([&]() { Vec v1(data.begin(), data.end()); Vec v2; v2 = v1; }));

  bench("assign_range", std::function<void()>([&]() { Vec v; v.assign(data.begin(), data.end()); }));
  bench("assign_fill", std::function<void()>([&]() { Vec v; v.assign(count, T()); }));

  bench("get_allocator", std::function<void()>([&]() { Vec v; typename Vec::allocator_type a = v.get_allocator(); (void)a; }));

  // Iterators
  bench("begin_end_iter", std::function<void()>([&]() {
    Vec v(data.begin(), data.end());
    for (typename Vec::iterator it = v.begin(); it != v.end(); ++it) { volatile T x = *it; (void)x; }
  }));
  bench("rbegin_rend_iter", std::function<void()>([&]() {
    Vec v(data.begin(), data.end());
    for (typename Vec::reverse_iterator it = v.rbegin(); it != v.rend(); ++it) { volatile T x = *it; (void)x; }
  }));

  // Capacity
  bench("size", std::function<void()>([&]() { Vec v(data.begin(), data.end()); volatile std::size_t s = v.size(); (void)s; }));
  bench("max_size", std::function<void()>([&]() { Vec v; volatile std::size_t m = v.max_size(); (void)m; }));
  bench("capacity", std::function<void()>([&]() { Vec v(data.begin(), data.end()); volatile std::size_t c = v.capacity(); (void)c; }));
  bench("reserve", std::function<void()>([&]() { Vec v; v.reserve(count); }));
  bench("resize_down", std::function<void()>([&]() { Vec v(data.begin(), data.end()); v.resize(count / 2); }));
  bench("resize_up", std::function<void()>([&]() { Vec v(data.begin(), data.end()); v.resize(count * 2, T()); }));
  bench("empty", std::function<void()>([&]() { Vec v; volatile bool e = v.empty(); (void)e; }));

  // Element access
  bench("element_access_operator", std::function<void()>([&]() {
    Vec v(data.begin(), data.end());
    for (std::size_t i = 0; i < v.size(); ++i) { volatile T x = v[i]; (void)x; }
  }));
  bench("element_access_at", std::function<void()>([&]() {
    Vec v(data.begin(), data.end());
    for (std::size_t i = 0; i < v.size(); ++i) { volatile T x = v.at(i); (void)x; }
  }));
  bench("front_back", std::function<void()>([&]() {
    Vec v(data.begin(), data.end());
    volatile T f = v.front(); (void)f;
    volatile T b = v.back(); (void)b;
  }));

  // Modifiers
  bench("push_back", std::function<void()>([&]() {
    Vec v;
    for (std::size_t i = 0; i < count; ++i) v.push_back(data[i]);
  }));
  bench("pop_back", std::function<void()>([&]() {
    Vec v(data.begin(), data.end());
    for (std::size_t i = 0; i < count; ++i) v.pop_back();
  }));

  bench("insert_single", std::function<void()>([&]() {
    Vec v(data.begin(), data.end());
    v.insert(v.begin() + v.size() / 2, T());
  }));
  bench("insert_fill", std::function<void()>([&]() {
    Vec v(data.begin(), data.end());
    v.insert(v.begin() + v.size() / 2, 10, T());
  }));
  bench("insert_range", std::function<void()>([&]() {
    Vec v(data.begin(), data.end());
    v.insert(v.begin() + v.size() / 2, data.begin(), data.begin() + std::min(count, std::size_t(10)));
  }));

  bench("erase_single", std::function<void()>([&]() {
    Vec v(data.begin(), data.end());
    v.erase(v.begin() + v.size() / 2);
  }));
  bench("erase_range", std::function<void()>([&]() {
    Vec v(data.begin(), data.end());
    v.erase(v.begin() + v.size() / 4, v.begin() + v.size() / 2);
  }));

  bench("swap", std::function<void()>([&]() {
    Vec v1(data.begin(), data.end());
    Vec v2(count, T());
    v1.swap(v2);
  }));

  bench("clear", std::function<void()>([&]() {
    Vec v(data.begin(), data.end());
    v.clear();
  }));
}

// Benchmark all C++98 member functions (timing only, no return validation)
template <typename Map, typename T>
void benchmark_cpp98_map(const std::string& ns_label, const std::string& type_label, std::size_t count, std::ofstream& out) {
  std::vector<T> data = generate_data<T>(count);
  std::string prefix = "map," + type_label;

  typedef typename Map::mapped_type  Mapped;
  typedef typename Map::value_type   Value; // usually pair<const Key, Mapped>

  auto bench = [&](const std::string& name, const std::function<void()>& func) {
    double t = measure_time(func);
    out << prefix << "," << name << "," << count << "," << ns_label << "," << t << "\n";
  };

  auto make_value = [&](const T& t) -> Value {
    return Value(t.first, t.second);
  };

  // Construct/copy/assign/insert
  bench("constructor_default", std::function<void()>([&]() { Map v; }));
  bench("constructor_range", std::function<void()>([&]() { Map v(data.begin(), data.end()); }));
  bench("constructor_copy", std::function<void()>([&]() { Map v1(data.begin(), data.end()); Map v2(v1); }));
  bench("operator_assign", std::function<void()>([&]() { Map v1(data.begin(), data.end()); Map v2; v2 = v1; }));

  bench("get_allocator", std::function<void()>([&]() { Map v; typename Map::allocator_type a = v.get_allocator(); (void)a; }));

  // Iterators
  bench("begin_end_iter", std::function<void()>([&]() {
    Map v(data.begin(), data.end());
    for (typename Map::iterator it = v.begin(); it != v.end(); ++it) { volatile T x = *it; (void)x; }
  }));
  bench("rbegin_rend_iter", std::function<void()>([&]() {
    Map v(data.begin(), data.end());
    for (typename Map::reverse_iterator it = v.rbegin(); it != v.rend(); ++it) { volatile T x = *it; (void)x; }
  }));

  // Capacity
  bench("size", std::function<void()>([&]() {
    Map v(data.begin(), data.end());
    volatile std::size_t s = v.size();
    (void)s;
  }));
  bench("max_size", std::function<void()>([&]() {
    Map v;
    volatile std::size_t m = v.max_size();
   (void)m;
  }));
  bench("empty", std::function<void()>([&]() {
    Map v;
    volatile bool e = v.empty();
    (void)e;
  }));

  // Element access
  bench("element_access_operator", std::function<void()>([&]() {
    Map v(data.begin(), data.end());
    for (std::size_t i = 0; i < data.size(); ++i) {
        volatile Mapped x = v[data[i].first]; // use the KEY, not index
        (void)x;
    }
  }));
  bench("element_access_at", std::function<void()>([&]() {
    Map v(data.begin(), data.end());
    for (std::size_t i = 0; i < data.size(); ++i) {
        volatile Mapped x = v.at(data[i].first); // use the KEY
        (void)x;
    }
  }));
  bench("uppter and lower bounds", std::function<void()>([&]() {
    Map v(data.begin(), data.end());
    {
        typename Map::iterator u = v.upper_bound(data[1].first); (void)u;
        typename Map::iterator l = v.lower_bound(data[1].first); (void)l;
    }
    {
        typename Map::const_iterator u = v.upper_bound(data[count - 2].first); (void)u;
        typename Map::const_iterator b = v.lower_bound(data[count - 2].first); (void)b;
    }
  }));

  // Iterate pairs (this is where you legitimately get pairs back)
  bench("iterate values", std::function<void()>([&]() {
    Map v(data.begin(), data.end());
    for (typename Map::const_iterator it = v.begin(); it != v.end(); ++it) {
        volatile Value kv = *it; // pair<const Key, Mapped>
        (void)kv;
    }
  }));

  // insert(value)
  bench("insert_value", std::function<void()>([&]() {
      Map v;
      for (std::size_t i = 0; i < count; ++i)
          v.insert(make_value(data[i]));
  }));

  // insert(hint, value)
  bench("insert_hint", std::function<void()>([&]() {
      Map v;
      typename Map::iterator hint = v.begin(); // arbitrary; fine for first insert
      for (std::size_t i = 0; i < count; ++i)
          hint = v.insert(hint, make_value(data[i]));
  }));

  // insert range  (there is NO "fill" insert for map)
  bench("insert_range", std::function<void()>([&]() {
      Map v;
      v.insert(data.begin(), data.end());
  }));

  // erase(first, last)
  bench("erase_range", std::function<void()>([&]() {
      Map v(data.begin(), data.end());
      typename Map::iterator a = v.begin();
      typename Map::iterator b = v.begin();
      std::advance(a, v.size() / 4);
      std::advance(b, v.size() / 2);
      v.erase(a, b);

      a = v.begin();
      b = v.begin();
      std::advance(a, v.size() / 4);
      std::advance(b, v.size() / 2);
      v.erase(a, b);
  }));

  // // erase(key)
  // bench("erase_all_keys", std::function<void()>([&]() {
  //     Map v(data.begin(), data.end());
  //     for (std::size_t i = 0; i < count; ++i)
  //         v.erase(data[i].first);
  // }));

  // insert duplicate keys (to see no-op cost in map vs multimap)
  bench("insert_dupe_keys", std::function<void()>([&]() {
      Map v;
      for (std::size_t i = 0; i < count; ++i) {
          Value val(data[0].first, data[i].second); // same key
          v.insert(val); // map: many no-ops; multimap: inserts
      }
  }));

  bench("swap", std::function<void()>([&]() {
      Map v1(data.begin(), data.end());
      Map v2(data.begin() + 1, data.end() - 1);
      v1.swap(v2);
  }));

  bench("clear", std::function<void()>([&]() {
      Map v(data.begin(), data.end());
      v.clear();
  }));
  }

  // Benchmark all C++98 member functions (timing only, no return validation)
  template <typename Map, typename T>
  void benchmark_cpp98_multi_map(const std::string& ns_label, const std::string& type_label, std::size_t count, std::ofstream& out) {
    std::vector<T> data = generate_data<T>(count);
    std::string prefix = "multimap," + type_label;

    typedef typename Map::value_type   Value; // usually pair<const Key, Mapped>

    auto bench = [&](const std::string& name, const std::function<void()>& func) {
      double t = measure_time(func);
      out << prefix << "," << name << "," << count << "," << ns_label << "," << t << "\n";
    };

    auto make_value = [&](const T& t) -> Value {
      return Value(t.first, t.second);
    };

    // Construct/copy/assign/insert
    bench("constructor_default", std::function<void()>([&]() { Map v; }));
    bench("constructor_range", std::function<void()>([&]() { Map v(data.begin(), data.end()); }));
    bench("constructor_copy", std::function<void()>([&]() { Map v1(data.begin(), data.end()); Map v2(v1); }));
    bench("operator_assign", std::function<void()>([&]() { Map v1(data.begin(), data.end()); Map v2; v2 = v1; }));

    bench("get_allocator", std::function<void()>([&]() { Map v; typename Map::allocator_type a = v.get_allocator(); (void)a; }));

    // Iterators
    bench("begin_end_iter", std::function<void()>([&]() {
      Map v(data.begin(), data.end());
      for (typename Map::iterator it = v.begin(); it != v.end(); ++it) { volatile T x = *it; (void)x; }
    }));
    bench("rbegin_rend_iter", std::function<void()>([&]() {
      Map v(data.begin(), data.end());
      for (typename Map::reverse_iterator it = v.rbegin(); it != v.rend(); ++it) { volatile T x = *it; (void)x; }
    }));

    // Capacity
    bench("size", std::function<void()>([&]() {
      Map v(data.begin(), data.end());
      volatile std::size_t s = v.size();
      (void)s;
    }));
    bench("max_size", std::function<void()>([&]() {
      Map v;
      volatile std::size_t m = v.max_size();
    (void)m;
    }));
    bench("empty", std::function<void()>([&]() {
      Map v;
      volatile bool e = v.empty();
      (void)e;
    }));

    // Element access
    bench("uppter and lower bounds", std::function<void()>([&]() {
      Map v(data.begin(), data.end());
      {
          typename Map::iterator u = v.upper_bound(data[1].first); (void)u;
          typename Map::iterator l = v.lower_bound(data[1].first); (void)l;
      }
      {
          typename Map::const_iterator u = v.upper_bound(data[count - 2].first); (void)u;
          typename Map::const_iterator b = v.lower_bound(data[count - 2].first); (void)b;
      }
    }));

    // Iterate pairs (this is where you legitimately get pairs back)
    bench("iterate values", std::function<void()>([&]() {
      Map v(data.begin(), data.end());
      for (typename Map::const_iterator it = v.begin(); it != v.end(); ++it) {
          volatile Value kv = *it; // pair<const Key, Mapped>
          (void)kv;
      }
    }));

    // insert(value)
    bench("insert_value", std::function<void()>([&]() {
        Map v;
        for (std::size_t i = 0; i < count; ++i)
            v.insert(make_value(data[i]));
    }));

    // insert(hint, value)
    bench("insert_hint", std::function<void()>([&]() {
        Map v;
        typename Map::iterator hint = v.begin(); // arbitrary; fine for first insert
        for (std::size_t i = 0; i < count; ++i)
            hint = v.insert(hint, make_value(data[i]));
    }));

    // insert range  (there is NO "fill" insert for map)
    bench("insert_range", std::function<void()>([&]() {
        Map v;
        v.insert(data.begin(), data.end());
    }));

    // erase(first, last)
    bench("erase_range", std::function<void()>([&]() {
        Map v(data.begin(), data.end());
        typename Map::iterator a = v.begin();
        typename Map::iterator b = v.begin();
        std::advance(a, v.size() / 4);
        std::advance(b, v.size() / 2);
        v.erase(a, b);

        a = v.begin();
        b = v.begin();
        std::advance(a, v.size() / 4);
        std::advance(b, v.size() / 2);
        v.erase(a, b);
    }));

    // // erase(key)
    // bench("erase_all_keys", std::function<void()>([&]() {
    //     Map v(data.begin(), data.end());
    //     for (std::size_t i = 0; i < count; ++i)
    //         v.erase(data[i].first);
    // }));

    // insert duplicate keys (to see no-op cost in map vs multimap)
    bench("insert_dupe_keys", std::function<void()>([&]() {
        Map v;
        for (std::size_t i = 0; i < count; ++i) {
            Value val(data[0].first, data[i].second); // same key
            v.insert(val); // map: many no-ops; multimap: inserts
        }
    }));

    bench("swap", std::function<void()>([&]() {
        Map v1(data.begin(), data.end());
        Map v2(data.begin() + 1, data.end() - 1);
        v1.swap(v2);
    }));

    bench("clear", std::function<void()>([&]() {
        Map v(data.begin(), data.end());
        v.clear();
    }));
  }


  // Benchmark all C++98 member functions (timing only, no return validation)
  template <typename Set, typename T>
  void benchmark_cpp98_set(const std::string& container_name, const std::string& ns_label, const std::string& type_label, std::size_t count, std::ofstream& out) {
    std::vector<T> data = generate_data<T>(count);
    std::string prefix = container_name + "," + type_label;

    typedef typename Set::value_type   Value; // usually pair<const Key>

    auto bench = [&](const std::string& name, const std::function<void()>& func) {
      double t = measure_time(func);
      out << prefix << "," << name << "," << count << "," << ns_label << "," << t << "\n";
    };

    auto make_value = [&](const T& t) -> Value {
      return Value(t);
    };

    // Construct/copy/assign/insert
    bench("constructor_default", std::function<void()>([&]() { Set v; }));
    bench("constructor_range", std::function<void()>([&]() { Set v(data.begin(), data.end()); }));
    bench("constructor_copy", std::function<void()>([&]() { Set v1(data.begin(), data.end()); Set v2(v1); }));
    bench("operator_assign", std::function<void()>([&]() { Set v1(data.begin(), data.end()); Set v2; v2 = v1; }));

    bench("get_allocator", std::function<void()>([&]() { Set v; typename Set::allocator_type a = v.get_allocator(); (void)a; }));

    // Iterators
    bench("begin_end_iter", std::function<void()>([&]() {
      Set v(data.begin(), data.end());
      for (typename Set::iterator it = v.begin(); it != v.end(); ++it) { volatile T x = *it; (void)x; }
    }));
    bench("rbegin_rend_iter", std::function<void()>([&]() {
      Set v(data.begin(), data.end());
      for (typename Set::reverse_iterator it = v.rbegin(); it != v.rend(); ++it) { volatile T x = *it; (void)x; }
    }));

    // Capacity
    bench("size", std::function<void()>([&]() {
      Set v(data.begin(), data.end());
      volatile std::size_t s = v.size();
      (void)s;
    }));
    bench("max_size", std::function<void()>([&]() {
      Set v;
      volatile std::size_t m = v.max_size();
    (void)m;
    }));
    bench("empty", std::function<void()>([&]() {
      Set v;
      volatile bool e = v.empty();
      (void)e;
    }));

    // Element access
    bench("uppter and lower bounds", std::function<void()>([&]() {
      Set v(data.begin(), data.end());
      {
          typename Set::iterator u = v.upper_bound(data[1]); (void)u;
          typename Set::iterator l = v.lower_bound(data[1]); (void)l;
      }
      {
          typename Set::const_iterator u = v.upper_bound(data[count - 2]); (void)u;
          typename Set::const_iterator b = v.lower_bound(data[count - 2]); (void)b;
      }
    }));

    // Iterate pairs (this is where you legitimately get pairs back)
    bench("iterate values", std::function<void()>([&]() {
      Set v(data.begin(), data.end());
      for (typename Set::const_iterator it = v.begin(); it != v.end(); ++it) {
          volatile Value kv = *it; // pair<const Key, Setped>
          (void)kv;
      }
    }));

    // insert(value)
    bench("insert_value", std::function<void()>([&]() {
        Set v;
        for (std::size_t i = 0; i < count; ++i)
            v.insert(make_value(data[i]));
    }));

    // insert(hint, value)
    bench("insert_hint", std::function<void()>([&]() {
        Set v;
        typename Set::iterator hint = v.begin(); // arbitrary; fine for first insert
        for (std::size_t i = 0; i < count; ++i)
            hint = v.insert(hint, make_value(data[i]));
    }));

    // insert range  (there is NO "fill" insert for map)
    bench("insert_range", std::function<void()>([&]() {
        Set v;
        v.insert(data.begin(), data.end());
    }));

    // erase(first, last)
    bench("erase_range", std::function<void()>([&]() {
        Set v(data.begin(), data.end());
        typename Set::iterator a = v.begin();
        typename Set::iterator b = v.begin();
        std::advance(a, v.size() / 4);
        std::advance(b, v.size() / 2);
        v.erase(a, b);

        a = v.begin();
        b = v.begin();
        std::advance(a, v.size() / 4);
        std::advance(b, v.size() / 2);
        v.erase(a, b);
    }));

    // // erase(key)
    // bench("erase_all_keys", std::function<void()>([&]() {
    //     Set v(data.begin(), data.end());
    //     for (std::size_t i = 0; i < count; ++i)
    //         v.erase(data[i].first);
    // }));

    // insert duplicate keys (to see no-op cost in map vs multimap)
    bench("insert_dupe_keys", std::function<void()>([&]() {
        Set v;
        for (std::size_t i = 0; i < count; ++i) {
            v.insert(make_value(data[i])); // map: many no-ops; multimap: inserts
        }
    }));

    bench("swap", std::function<void()>([&]() {
        Set v1(data.begin(), data.end());
        Set v2(data.begin() + 1, data.end() - 1);
        v1.swap(v2);
    }));

    bench("clear", std::function<void()>([&]() {
        Set v(data.begin(), data.end());
        v.clear();
    }));
  }

} // namespace benchmark

int main() {
  using namespace benchmark;
  std::srand(static_cast<unsigned>(std::time(0)));
  std::ofstream csv("benchmark_results.csv");
  csv << "Container,Type,Function,Size,Namespace,Time\n";
  std::size_t sizes[] = {1000, 10000, 100000};

  { // VECTOR
  using ft_vec_int = ft::vector<int>;
  using std_vec_int = std::vector<int>;
  using ft_vec_str = ft::vector<std::string>;
  using std_vec_str = std::vector<std::string>;
  using ft_vec_point = ft::vector<Point>;
  using std_vec_point = std::vector<Point>;

  for (std::size_t size : sizes) {
    benchmark_cpp98_vector<ft_vec_int, int>("ft", "int", size, csv);
    benchmark_cpp98_vector<std_vec_int, int>("std", "int", size, csv);

    benchmark_cpp98_vector<ft_vec_str, std::string>("ft", "string", size, csv);
    benchmark_cpp98_vector<std_vec_str, std::string>("std", "string", size, csv);

    benchmark_cpp98_vector<ft_vec_point, Point>("ft", "point", size, csv);
    benchmark_cpp98_vector<std_vec_point, Point>("std", "point", size, csv);
  }
  }

  { // DEQUE
  using ft_deque_int = ft::deque<int>;
  using std_deque_int = std::deque<int>;
  using ft_deque_string = ft::deque<std::string>;
  using std_deque_string = std::deque<std::string>;
  using ft_deque_point = ft::deque<Point>;
  using std_deque_point = std::deque<Point>;

  for (std::size_t size : sizes) {
    benchmark_cpp98_deque<ft_deque_int, int>("ft", "int", size, csv);
    benchmark_cpp98_deque<std_deque_int, int>("std", "int", size, csv);

    benchmark_cpp98_deque<ft_deque_string, std::string>("ft", "string", size, csv);
    benchmark_cpp98_deque<std_deque_string, std::string>("std", "string", size, csv);

    benchmark_cpp98_deque<ft_deque_point, Point>("ft", "point", size, csv);
    benchmark_cpp98_deque<std_deque_point, Point>("std", "point", size, csv);
  }

  }

  { // LIST
  using ft_list_int = ft::list<int>;
  using std_list_int = std::list<int>;
  using ft_list_string = ft::list<std::string>;
  using std_list_string = std::list<std::string>;
  using ft_list_point = ft::list<Point>;
  using std_list_point = std::list<Point>;

  for (std::size_t size : sizes) {
    benchmark_cpp98_list<ft_list_int, int>("ft", "int", size, csv);
    benchmark_cpp98_list<std_list_int, int>("std", "int", size, csv);

    benchmark_cpp98_list<ft_list_string, std::string>("ft", "string", size, csv);
    benchmark_cpp98_list<std_list_string, std::string>("std", "string", size, csv);

    benchmark_cpp98_list<ft_list_point, Point>("ft", "point", size, csv);
    benchmark_cpp98_list<std_list_point, Point>("std", "point", size, csv);
  }
  }

  { // MAP
  using ft_list_int = ft::map<int, int>;
  using std_list_int = std::map<int, int>;
  using ft_list_string = ft::map<std::string, int>;
  using std_list_string = std::map<std::string, int>;
  using ft_list_point = ft::map<Point, int>;
  using std_list_point = std::map<Point, int>;

  for (std::size_t size : sizes) {
    benchmark_cpp98_map<ft_list_int, ft::pair<int, int>>("ft", "int", size, csv);
    benchmark_cpp98_map<std_list_int, std::pair<int, int>>("std", "int", size, csv);

    benchmark_cpp98_map<ft_list_string, ft::pair<std::string, int>>("ft", "string", size, csv);
    benchmark_cpp98_map<std_list_string, std::pair<std::string, int>>("std", "string", size, csv);

    benchmark_cpp98_map<ft_list_point, ft::pair<Point, int>>("ft", "point", size, csv);
    benchmark_cpp98_map<std_list_point, std::pair<Point, int>>("std", "point", size, csv);
  }
  }

  { // MULTIMAP
  using ft_list_int = ft::multimap<int, int>;
  using std_list_int = std::multimap<int, int>;
  using ft_list_string = ft::multimap<std::string, int>;
  using std_list_string = std::multimap<std::string, int>;
  using ft_list_point = ft::multimap<Point, int>;
  using std_list_point = std::multimap<Point, int>;

  for (std::size_t size : sizes) {
    benchmark_cpp98_multi_map<ft_list_int, ft::pair<int, int>>("ft", "int", size, csv);
    benchmark_cpp98_multi_map<std_list_int, std::pair<int, int>>("std", "int", size, csv);

    benchmark_cpp98_multi_map<ft_list_string, ft::pair<std::string, int>>("ft", "string", size, csv);
    benchmark_cpp98_multi_map<std_list_string, std::pair<std::string, int>>("std", "string", size, csv);

    benchmark_cpp98_multi_map<ft_list_point, ft::pair<Point, int>>("ft", "point", size, csv);
    benchmark_cpp98_multi_map<std_list_point, std::pair<Point, int>>("std", "point", size, csv);
  }
  }

  { // SET
  using ft_list_int = ft::set<int>;
  using std_list_int = std::set<int>;
  using ft_list_string = ft::set<std::string>;
  using std_list_string = std::set<std::string>;
  using ft_list_point = ft::set<Point>;
  using std_list_point = std::set<Point>;

  for (std::size_t size : sizes) {
    benchmark_cpp98_set<ft_list_int, int>("set", "ft", "int", size, csv);
    benchmark_cpp98_set<std_list_int, int>("set", "std", "int", size, csv);

    benchmark_cpp98_set<ft_list_string, std::string>("set", "ft", "string", size, csv);
    benchmark_cpp98_set<std_list_string, std::string>("set", "std", "string", size, csv);

    benchmark_cpp98_set<ft_list_point, Point>("set", "ft", "point", size, csv);
    benchmark_cpp98_set<std_list_point, Point>("set", "std", "point", size, csv);
  }
  }

  { // MULTISET
  using ft_list_int = ft::multiset<int>;
  using std_list_int = std::multiset<int>;
  using ft_list_string = ft::multiset<std::string>;
  using std_list_string = std::multiset<std::string>;
  using ft_list_point = ft::multiset<Point>;
  using std_list_point = std::multiset<Point>;

  for (std::size_t size : sizes) {
    benchmark_cpp98_set<ft_list_int, int>("multiset", "ft", "int", size, csv);
    benchmark_cpp98_set<std_list_int, int>("multiset", "std", "int", size, csv);

    benchmark_cpp98_set<ft_list_string, std::string>("multiset", "ft", "string", size, csv);
    benchmark_cpp98_set<std_list_string, std::string>("multiset", "std", "string", size, csv);

    benchmark_cpp98_set<ft_list_point, Point>("multiset", "ft", "point", size, csv);
    benchmark_cpp98_set<std_list_point, Point>("multiset", "std", "point", size, csv);
  }
  }
  csv.close();
  std::cout << "✅ Full C++98 benchmark written to benchmark_results.csv\n";
  return 0;
}
