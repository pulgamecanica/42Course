#include <iostream>
#include <fstream>
#include <vector>
#include <deque>
#include <list>
#include <string>
#include <ctime>
#include <cstdlib>
#include <functional>
#include "vector.hpp"
#include "deque.hpp"
#include "list.hpp"

namespace benchmark {


struct Point {
    int x, y;
    Point(int a = 0, int b = 0) : x(a), y(b) {}

    // Needed for remove_if and unique
    bool operator==(const Point& other) const {
        return x == other.x && y == other.y;
    }

    // Needed for merge(comp) and sort(comp)
    bool operator<(const Point& other) const {
        return x != other.x ? x < other.x : y < other.y;
    }
};

// Utility to measure execution time
template <typename Func>
double measure_time(Func f) {
    clock_t start = clock();
    f();
    clock_t end = clock();
    return static_cast<double>(end - start) / CLOCKS_PER_SEC;
}

// Data generators
template <typename T>
std::vector<T> generate_data(std::size_t count);

template <>
std::vector<int> generate_data<int>(std::size_t count) {
    std::vector<int> data;
    for (std::size_t i = 0; i < count; ++i)
        data.push_back(rand());
    return data;
}

template <>
std::vector<std::string> generate_data<std::string>(std::size_t count) {
    std::vector<std::string> data;
    for (std::size_t i = 0; i < count; ++i)
        data.push_back("str_" + std::to_string(rand() % 10000));
    return data;
}

template <>
std::vector<Point> generate_data<Point>(std::size_t count) {
    std::vector<Point> data;
    for (std::size_t i = 0; i < count; ++i)
        data.push_back(Point(rand() % 1000, rand() % 1000));
    return data;
}

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
    std::string benchmark_prefix = "vector," + type_label;

    double t_default_ctor = measure_time([&]() {
        Vec v;
    });
    out << benchmark_prefix << ",constructor_default," << count << "," << ns_label << "," << t_default_ctor << "\n";

    double t_fill_ctor = measure_time([&]() {
        Vec v(count, T());
    });
    out << benchmark_prefix << ",constructor_fill," << count << "," << ns_label << "," << t_fill_ctor << "\n";

    double t_range_ctor = measure_time([&]() {
        Vec v(data.begin(), data.end());
    });
    out << benchmark_prefix << ",constructor_range," << count << "," << ns_label << "," << t_range_ctor << "\n";

    double t_copy_ctor = measure_time([&]() {
        Vec v1(data.begin(), data.end());
        Vec v2(v1);
    });
    out << benchmark_prefix << ",constructor_copy," << count << "," << ns_label << "," << t_copy_ctor << "\n";

    double t_assignment = measure_time([&]() {
        Vec v1(count, T());
        Vec v2;
        v2 = v1;
    });
    out << benchmark_prefix << ",operator_assign," << count << "," << ns_label << "," << t_assignment << "\n";

    double t_push_back = measure_time([&]() {
        Vec v;
        for (std::size_t i = 0; i < count; ++i) v.push_back(data[i]);
    });
    out << benchmark_prefix << ",push_back," << count << "," << ns_label << "," << t_push_back << "\n";

    double t_insert = measure_time([&]() {
        Vec v(data.begin(), data.end());
        v.insert(v.begin() + count / 2, T());
    });
    out << benchmark_prefix << ",insert_middle," << count << "," << ns_label << "," << t_insert << "\n";

    double t_resize = measure_time([&]() {
        Vec v(data.begin(), data.end());
        v.resize(count / 2);
    });
    out << benchmark_prefix << ",resize_down," << count << "," << ns_label << "," << t_resize << "\n";

    double t_reserve = measure_time([&]() {
        Vec v;
        v.reserve(count);
    });
    out << benchmark_prefix << ",reserve," << count << "," << ns_label << "," << t_reserve << "\n";

    double t_assign_range = measure_time([&]() {
        Vec v;
        v.assign(data.begin(), data.end());
    });
    out << benchmark_prefix << ",assign_range," << count << "," << ns_label << "," << t_assign_range << "\n";

    double t_clear = measure_time([&]() {
        Vec v(data.begin(), data.end());
        v.clear();
    });
    out << benchmark_prefix << ",clear," << count << "," << ns_label << "," << t_clear << "\n";

    double t_erase_range = measure_time([&]() {
        Vec v(data.begin(), data.end());
        v.erase(v.begin(), v.begin() + v.size() / 2);
    });
    out << benchmark_prefix << ",erase_range," << count << "," << ns_label << "," << t_erase_range << "\n";

    double t_swap = measure_time([&]() {
        Vec v1(data.begin(), data.end());
        Vec v2(count, T());
        v1.swap(v2);
    });
    out << benchmark_prefix << ",swap," << count << "," << ns_label << "," << t_swap << "\n";
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
    csv.close();
    std::cout << "✅ Full C++98 benchmark written to benchmark_results.csv\n";
    return 0;
}
