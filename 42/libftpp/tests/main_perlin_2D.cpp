#include <iostream>
#include <cmath>
#include "perlin_noise_2D.hpp"

int main() {
    PerlinNoise2D perlin;

    const int gridSize = 10;
    char visualChars[] = {' ', '.', ':', '-', '=', '+', '*', '#', '%', '@'};

    std::cout << "Sampling 2D Perlin noise over a " << gridSize << "x" << gridSize << " grid:" << std::endl << std::endl;

    for (int y = 0; y < gridSize; ++y) {
        for (int x = 0; x < gridSize; ++x) {
            float sample = perlin.sample(x * 0.1f, y * 0.1f); // Adjust these factors as needed
            sample = (sample + 1) / 2; // Map from [-1, 1] to [0, 1]
            int charIndex = std::round(sample * 9); // Map from [0, 1] to [0, 9]

            std::cout << visualChars[charIndex] << " ";
        }
        std::cout << std::endl;
    }

    return 0;
}

