#include "ex01_bonus.inc"
#include <cmath> // For abs
#include <sstream> // stringstream

namespace ex01_bonus {
  uint32_t parse_color(const std::string& color) {
    if (color[0] != '#' || (color.size() != 4 && color.size() != 7 && color.size() != 9)) {
      throw std::invalid_argument("Invalid color format");
    }
    std::istringstream ss;
    ss >> std::hex;
    uint32_t red, green, blue, alpha;

    if (color.size() == 4) { // #RGB format
        ss.str(color.substr(1, 1) + color.substr(1, 1));
        ss >> red;
        ss.clear();
        ss.str(color.substr(2, 1) + color.substr(2, 1));
        ss >> green;
        ss.clear();
        ss.str(color.substr(3, 1) + color.substr(3, 1));
        ss >> blue;
        ss.clear();
        alpha = 0xFF; // Default alpha value if not provided
    } else { // #RRGGBB & #RRGGBBAA format
        ss.str(color.substr(1, 2));
        ss >> red;
        ss.clear();
        ss.str(color.substr(3, 2));
        ss >> green;
        ss.clear();
        ss.str(color.substr(5, 2));
        ss >> blue;
        alpha = 0xFF; // Default alpha value if not provided
        ss.clear();
        if (color.size() == 9)  { // #RRGGBBAA format
          ss.str(color.substr(7, 2));
          ss >> alpha;
        }
    }
    return (alpha << 24) | (red << 16) | (green << 8) | blue;
  }

  int gen_trgb(int opacity, int red, int green, int blue) {
    if (opacity > 255 || red > 255 || green > 255 || blue > 255)
      return (0);
    return (opacity << 24 | red << 16 | green << 8 | blue);
  }

  int get_opacity(int trgb) {
    return ((trgb >> 24) & 0XFF);
  }

  int get_r(int trgb) {
    return ((trgb >> 16) & 0XFF);
  }

  int get_g(int trgb) {
    return ((trgb >> 8) & 0XFF);
  }

  int get_b(int trgb) {
    return (trgb & 0xFF);
  }

  void draw_line(std::vector<unsigned char>& image, int x0, int y0, int x1, int y1, uint32_t color, unsigned width) {
      (void)color;
      int dx = abs(x1-x0), sx = x0<x1 ? 1 : -1;
      int dy = abs(y1-y0), sy = y0<y1 ? 1 : -1; 
      int err = (dx>dy ? dx : -dy)/2, e2;
      for(;;){
        unsigned pos = 4 * y0 * width + 4 * x0;
        if (pos+4 > image.size()) // Protect out of bounds
          return ;
        image[pos + 0] = get_r(color);
        image[pos + 1] = get_g(color);
        image[pos + 2] = get_b(color);
        image[pos + 3] = get_opacity(color);
        if (x0==x1 && y0==y1)
              break;
        e2 = err;
        if (e2 >-dx) {
              err -= dy;
              x0 += sx;
        }
        if (e2 < dy) {
              err += dx;
              y0 += sy;
        }
      }
  } 

  void draw_circle_coords(std::vector<unsigned char>& image, int xc, int yc, int x, int y, uint32_t color, unsigned width) {
      draw_line(image, xc+x, yc+y, xc-x, yc+y, color, width);
      draw_line(image, xc+x, yc-y, xc-x, yc-y, color, width);
      draw_line(image, xc+y, yc+x, xc-y, yc+x, color, width);
      draw_line(image, xc+y, yc-x, xc-y, yc-x, color, width);
  }

  void draw_circle(std::vector<unsigned char>& image, int xc, int yc, int r, uint32_t color, unsigned width) {
      int x = 0, y = r;
      int d = 3 - 2 * r;
      while (y >= x) {
          /*for each pixel we will draw all eight pixels */
          draw_circle_coords(image, xc, yc, x, y, color, width);
          x++;
          /*check for decision parameter and correspondingly update d, x, y*/
          if (d > 0) {
              y--;
              d = d + 4 * (x - y) + 10;
          } else {
              d = d + 4 * x + 6;
          }
          draw_circle_coords(image, xc, yc, x, y, color, width);
      }
  }
}
