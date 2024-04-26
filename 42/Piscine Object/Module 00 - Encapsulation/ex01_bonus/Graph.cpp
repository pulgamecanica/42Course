//***************************//
//*Template by pulgamecanica*//
//***************************//

extern "C" {
    #include <sys/ioctl.h>
    #include <unistd.h>
    #include <stdint.h> // for uint32_t
    #include <stdio.h>
}

#include <iostream>
#include <fstream> // ifstream
#include <sstream> // istringstream
#include <algorithm> // max

#include "ex01_bonus.inc"
#include "Graph.hpp"

#include "lodepng.h"

namespace ex01_bonus {
    Graph::Graph(const std::string& file): size_(Vector2(0, 0)) {
        std::cout << BLUE << "Graph\t" << GREEN << "NEW\t" << ENDC << size_ << std::endl;
        std::cout << "File\t" << BLUE << file << ENDC << " parsed" << std::endl;
        size_ = parse_file(file);;
    }

    Graph::Graph(const Vector2& size): size_(size) {
        std::cout << BLUE << "Graph\t" << GREEN << "NEW\t" << ENDC << size_ << std::endl;
    }

    Graph::~Graph() {
        points_.clear();
        lines_.clear();
        std::cout << BLUE << "Graph\t" << GREEN << "DELETE\t" << ENDC << size_ << std::endl;
    }

    const Vector2& Graph::get_size() const {
        return size_;
    }

    void Graph::save_to_png(const std::string& filename, float scale) const {
        std::vector<unsigned char> image;
        std::vector<unsigned char> png;
        unsigned w = size_.x() * scale;
        unsigned h = size_.y() * scale;

        if (scale <= 0) {
            std::cout << RED << "Error, scale should be > 0" << std::endl;
            return ;
        }
        std::cout << "Generating " << BLUE << filename << ENDC << " (" << size_ << ")px" << std::endl;
        image.resize(w * h * 4);
        float factor = 1/scale;
        for (unsigned y = 0; y < h; ++y)
        for (unsigned x = 0; x < w; ++x)
        {
            unsigned x_real = x * factor;
            unsigned y_real = y * factor;
            unsigned pos = 4 * y * w + 4 * x;
            if (has_point(Vector2(x_real, y_real))) {
                Vector2 v = *(std::find(points_.begin(), points_.end(), Vector2(x_real, y_real)));
                unsigned r, g, b, a;
                r = get_r(v.get_color());
                g = get_g(v.get_color());
                b = get_b(v.get_color());
                a = get_opacity(v.get_color());
                image[pos + 0] = r;   // R
                image[pos + 1] = g;     // G
                image[pos + 2] = b;     // B
                image[pos + 3] = a;   // A
            } else {
                image[pos + 0] = 42;    // R
                image[pos + 1] = 142;   // G
                image[pos + 2] = 242;   // B
                image[pos + 3] = 255;   // A
            }
        }
        for (std::vector<Line>::const_iterator i = lines_.begin(); i != lines_.end(); ++i) {
            Vector2 p1 = i->p1();
            Vector2 p2 = i->p2();
            draw_line(image, p1.x() * scale, p1.y() * scale, p2.x() * scale, p2.y() * scale, i->get_color(), w);
        }
        // uint32_t color = gen_trgb(255, 10, 200, 42);
        unsigned error = lodepng::encode(png, image, w, h);
        if(error) {
            std::cout << "PNG encoding error " << error << ": " << lodepng_error_text(error) << std::endl;
            return;
        }
        lodepng::save_file(png, filename);
        std::cout << GREEN << "Your image is now available at '" BLUE << filename << GREEN << "'" << ENDC << std::endl;
        image.clear();
        png.clear();
    }

    // All lines are allowed, even lines not inside the Graph
    void Graph::add_line(const Line & l) {
        std::cout << BLUE << "Graph\t" << GREEN << "ADD\t" << ENDC << l << std::endl;
        lines_.push_back(l);
    }

    // All points are allowed, even points that are not inside the Graph
    bool Graph::add_point(const Vector2 & p) {
        std::cout << BLUE << "Graph\t" << GREEN << "ADD\t" << ENDC << p << std::endl;
        points_.push_back(p);
        return true;
    }

    bool Graph::has_point(const Vector2& p) const {
        for (std::vector<Vector2>::const_iterator i = points_.begin(); i != points_.end(); ++i) {
            if (*i == p)
                return true;
        }
        return false;
    }

    Vector2 Graph::parse_file(const std::string& filename) {
        unsigned max_x, max_y;
        max_x = 0;
        max_y = 0;
        std::ifstream file(filename.c_str());
        if (!file.is_open()) {
            throw std::runtime_error("Error: Unable to open file");
        }
        std::string line;
        int i = 0;
        while (std::getline(file, line)) {
            i++;
            std::istringstream iss(line);
            std::string type;
            iss >> type;

            if (type == "point") {
                float x, y;
                std::string color_str;
                if (!(iss >> x >> y >> color_str)) {
                    std::cerr << RED << "Error" << ENDC << ": Invalid point format in line: " << line << std::endl;
                    continue;
                }
                uint32_t color;
                try {
                    color = parse_color(color_str);
                } catch (std::exception& e) {
                    std::cerr << RED << "Error" << ENDC << ": Invalid color format in line: " << line << std::endl;
                    continue;
                }
                points_.push_back(Vector2(x, y, color));
                max_x = std::max(max_x, (unsigned)x);
                max_y = std::max(max_y, (unsigned)y);
            } else if (type == "line") {
                float x1, y1, x2, y2;
                std::string color_str;
                if (!(iss >> x1 >> y1 >> x2 >> y2 >> color_str)) {
                    std::cerr << "Error: Invalid line format in line[" << BLUE << i << ENDC << "]: " << line << std::endl;
                    continue;
                }
                uint32_t color;
                try {
                    color = parse_color(color_str);
                } catch (std::exception& e) {
                    std::cerr << RED << "Error" << ENDC << ": Invalid color format in line[" << BLUE << i << ENDC << "]: " << line << std::endl;
                    continue;
                }
                lines_.push_back(Line(Vector2(x1, y1), Vector2(x2, y2), color));
                max_x = std::max(max_x, (unsigned)x1);
                max_x = std::max(max_x, (unsigned)x2);
                max_y = std::max(max_y, (unsigned)y1);
                max_y = std::max(max_y, (unsigned)y2);
            } else {
                if (line[0] == '#' || line.empty())
                    continue;
                std::cerr << "Error: Unknown type '" << type << "' in line[" << BLUE << i << ENDC << "]: " << line << std::endl;
            }
        }
        file.close();
        return Vector2(max_x + 1, max_y + 1);
    }

    std::ostream& operator<<(std::ostream& s, const Graph& g) {
        s << BLUE << "Graph " << ENDC << "size: " << g.get_size();
        return (s);
    }
}

 
