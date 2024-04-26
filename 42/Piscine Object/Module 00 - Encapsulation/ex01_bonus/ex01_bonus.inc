//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __EX01_BONUS_HPP__
# define __EX01_BONUS_HPP__

#ifndef DEBUG
# define DEBUG 0
#endif

extern "C" {
    #include <stdint.h> // for uint32_t
}

#include <vector>


#include <iostream>


# define BLACK  "\033[0;30m"
# define RED    "\033[0;31m"
# define GREEN  "\033[0;32m"
# define YELLOW "\033[0;33m"
# define BLUE   "\033[0;34m"
# define WHITE  "\033[0;37m"
# define ENDC   "\033[0m"

namespace ex01_bonus {
    uint32_t parse_color(const std::string& color);
    int gen_trgb(int opacity, int red, int green, int blue);
    int get_opacity(int trgb);
    int get_r(int trgb);
    int get_g(int trgb);
    int get_b(int trgb);
    void draw_line(std::vector<unsigned char>& image, int x0, int y0, int x1, int y1, uint32_t color, unsigned width);
    void draw_circle_coords(std::vector<unsigned char>& image, int xc, int yc, int x, int y, uint32_t color, unsigned width);
    void draw_circle(std::vector<unsigned char>& image, int xc, int yc, int r, uint32_t color, unsigned width);
}

#endif
