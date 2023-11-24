#include "utils.hpp"
#include <fstream>

uint64_t           gettimeofday_ms(void) {
    static struct timeval   tv;

    gettimeofday(&tv, 0);
    return ((tv.tv_sec * (uint64_t)1000) + (tv.tv_usec / 1000));
}

uint64_t        timestamp_in_ms(uint64_t originTime) {
    return (gettimeofday_ms() - originTime);
}

std::vector<char>   readFile(const std::string& filename) {
    std::ifstream file(filename, std::ios::ate | std::ios::binary);

    if (!file.is_open()) {
        throw std::runtime_error("failed to open file!");
    }

    size_t fileSize = (size_t) file.tellg();
    std::vector<char> buffer(fileSize);

    file.seekg(0);
    file.read(buffer.data(), fileSize);
    file.close();

    return buffer;
}
