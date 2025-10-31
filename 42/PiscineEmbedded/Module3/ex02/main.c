#include "pulga/builtin_rgb.h"

int main(void) {
    init_rgb();

    uint8_t pos = 0;
    while (1) {
        wheel(pos++);
        _delay_ms(10); // Adjust for transition speed
    }
}
