#include <iostream>
#include <vector>
#include <utility>
#include "random_2D_coordinate_generator.hpp"

int main() {
    Random2DCoordinateGenerator randomGenerator;

    // Store a list of coordinates to test
    std::vector<std::pair<long long, long long>> coordinates = {
        {5, 3},
        {7, 2},
        {1, 9},
        {0, 0}
    };

    // Store the random numbers generated the first time
    std::vector<long long> firstGenerated;

    std::cout << "First round of generation:" << std::endl;
    for (const auto& coord : coordinates) {
        long long x = coord.first;
        long long y = coord.second;
        long long randomNumber = randomGenerator(x, y);
        firstGenerated.push_back(randomNumber);
        std::cout << "Random number using coordinates (" << x << ", " << y << "): " << randomNumber << std::endl;
    }
    std::cout << std::endl;

    std::cout << "Second round of generation:" << std::endl;
    for (size_t i = 0; i < coordinates.size(); ++i) {
        long long x = coordinates[i].first;
        long long y = coordinates[i].second;
        long long randomNumber = randomGenerator(x, y);
        
        std::cout << "Random number using coordinates (" << x << ", " << y << "): " << randomNumber << std::endl;
        
        // Check if the number is the same as generated the first time
        if (randomNumber == firstGenerated[i]) {
            std::cout << "  => Matches the previous generated value. Consistent!" << std::endl;  // Expected: Should always match
        } else {
            std::cout << "  => Does not match the previous generated value. Inconsistent!" << std::endl;
        }
    }
    std::cout << std::endl;

    return 0;
}

