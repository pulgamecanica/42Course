#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <ctime>
#include <cstdlib>
#include "vector.hpp"

namespace benchmark {

struct Point {
    int x, y;
    Point(int a = 0, int b = 0) : x(a), y(b) {}
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

// Benchmark all C++98 member functions (timing only, no return validation)
template <typename Vec, typename T>
void benchmark_cpp98_vector(const std::string& ns_label, const std::string& type_label, std::size_t count, std::ofstream& out) {
    std::vector<T> data = generate_data<T>(count);

    double t_default_ctor = measure_time([&]() {
        Vec v;
    });
    out << type_label << ",constructor_default," << count << "," << ns_label << "," << t_default_ctor << "\n";

    double t_fill_ctor = measure_time([&]() {
        Vec v(count, T());
    });
    out << type_label << ",constructor_fill," << count << "," << ns_label << "," << t_fill_ctor << "\n";

    double t_range_ctor = measure_time([&]() {
        Vec v(data.begin(), data.end());
    });
    out << type_label << ",constructor_range," << count << "," << ns_label << "," << t_range_ctor << "\n";

    double t_copy_ctor = measure_time([&]() {
        Vec v1(data.begin(), data.end());
        Vec v2(v1);
    });
    out << type_label << ",constructor_copy," << count << "," << ns_label << "," << t_copy_ctor << "\n";

    double t_assignment = measure_time([&]() {
        Vec v1(count, T());
        Vec v2;
        v2 = v1;
    });
    out << type_label << ",operator_assign," << count << "," << ns_label << "," << t_assignment << "\n";

    double t_push_back = measure_time([&]() {
        Vec v;
        for (std::size_t i = 0; i < count; ++i) v.push_back(data[i]);
    });
    out << type_label << ",push_back," << count << "," << ns_label << "," << t_push_back << "\n";

    double t_insert = measure_time([&]() {
        Vec v(data.begin(), data.end());
        v.insert(v.begin() + count / 2, T());
    });
    out << type_label << ",insert_middle," << count << "," << ns_label << "," << t_insert << "\n";

    double t_resize = measure_time([&]() {
        Vec v(data.begin(), data.end());
        v.resize(count / 2);
    });
    out << type_label << ",resize_down," << count << "," << ns_label << "," << t_resize << "\n";

    double t_reserve = measure_time([&]() {
        Vec v;
        v.reserve(count);
    });
    out << type_label << ",reserve," << count << "," << ns_label << "," << t_reserve << "\n";

    double t_assign_range = measure_time([&]() {
        Vec v;
        v.assign(data.begin(), data.end());
    });
    out << type_label << ",assign_range," << count << "," << ns_label << "," << t_assign_range << "\n";

    double t_clear = measure_time([&]() {
        Vec v(data.begin(), data.end());
        v.clear();
    });
    out << type_label << ",clear," << count << "," << ns_label << "," << t_clear << "\n";

    double t_erase_range = measure_time([&]() {
        Vec v(data.begin(), data.end());
        v.erase(v.begin(), v.begin() + v.size() / 2);
    });
    out << type_label << ",erase_range," << count << "," << ns_label << "," << t_erase_range << "\n";

    double t_swap = measure_time([&]() {
        Vec v1(data.begin(), data.end());
        Vec v2(count, T());
        v1.swap(v2);
    });
    out << type_label << ",swap," << count << "," << ns_label << "," << t_swap << "\n";
}

} // namespace benchmark

int main() {
    using namespace benchmark;
    std::srand(static_cast<unsigned>(std::time(0)));

    using ft_vec_int = ft::vector<int>;
    using std_vec_int = std::vector<int>;
    using ft_vec_str = ft::vector<std::string>;
    using std_vec_str = std::vector<std::string>;
    using ft_vec_point = ft::vector<Point>;
    using std_vec_point = std::vector<Point>;

    std::ofstream csv("benchmark_results.csv");
    csv << "Type,Function,Size,Namespace,Time\n";

    std::size_t sizes[] = {1000, 10000, 1000000};

    for (std::size_t size : sizes) {
        benchmark_cpp98_vector<ft_vec_int, int>("ft", "int", size, csv);
        benchmark_cpp98_vector<std_vec_int, int>("std", "int", size, csv);

        benchmark_cpp98_vector<ft_vec_str, std::string>("ft", "string", size, csv);
        benchmark_cpp98_vector<std_vec_str, std::string>("std", "string", size, csv);

        benchmark_cpp98_vector<ft_vec_point, Point>("ft", "point", size, csv);
        benchmark_cpp98_vector<std_vec_point, Point>("std", "point", size, csv);
    }

    csv.close();
    std::cout << "✅ Full C++98 vector benchmark written to benchmark_results.csv\n";
    return 0;
}
