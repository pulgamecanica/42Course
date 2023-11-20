#include "utils.hpp"

uint64_t    gettimeofday_ms(void) {
    static struct timeval   tv;

    gettimeofday(&tv, 0);
    return ((tv.tv_sec * (uint64_t)1000) + (tv.tv_usec / 1000));
}

uint64_t    timestamp_in_ms(uint64_t originTime) {
    return (gettimeofday_ms() - originTime);
}