// benchmark.hpp
#include <vector>
#include <string>
#include <cstdlib>
#include <utility>
#include "utility.hpp"

#pragma once

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

template <>
std::vector<std::pair<int, int> > generate_data<std::pair<int, int>>(std::size_t count) {
    std::vector<std::pair<int, int>> data;
    for (std::size_t i = 0; i < count; ++i)
        data.push_back(std::make_pair<int, int>(rand() % 1000, rand() % 1000));
    return data;
}

template <>
std::vector<std::pair<std::string, int> > generate_data<std::pair<std::string, int>>(std::size_t count) {
    std::vector<std::pair<std::string, int>> data;
    for (std::size_t i = 0; i < count; ++i)
        data.push_back(std::make_pair<std::string, int>("str_" + std::to_string(rand() % 10000), rand() % 1000));
    return data;
}

template <>
std::vector<std::pair<Point, int> > generate_data<std::pair<Point, int>>(std::size_t count) {
    std::vector<std::pair<Point, int>> data;
    for (std::size_t i = 0; i < count; ++i)
        data.push_back(std::make_pair<Point, int>(Point(rand() % 1000, rand() % 1000), rand() % 1000));
    return data;
}

template <>
std::vector<ft::pair<int, int> > generate_data<ft::pair<int, int>>(std::size_t count) {
    std::vector<ft::pair<int, int>> data;
    for (std::size_t i = 0; i < count; ++i)
        data.push_back(ft::make_pair<int, int>(rand() % 1000, rand() % 1000));
    return data;
}

template <>
std::vector<ft::pair<std::string, int> > generate_data<ft::pair<std::string, int>>(std::size_t count) {
    std::vector<ft::pair<std::string, int>> data;
    for (std::size_t i = 0; i < count; ++i)
        data.push_back(ft::make_pair<std::string, int>("str_" + std::to_string(rand() % 10000), rand() % 1000));
    return data;
}

template <>
std::vector<ft::pair<Point, int> > generate_data<ft::pair<Point, int>>(std::size_t count) {
    std::vector<ft::pair<Point, int>> data;
    for (std::size_t i = 0; i < count; ++i)
        data.push_back(ft::make_pair<Point, int>(Point(rand() % 1000, rand() % 1000), rand() % 1000));
    return data;
}


} // namespace benchmark
