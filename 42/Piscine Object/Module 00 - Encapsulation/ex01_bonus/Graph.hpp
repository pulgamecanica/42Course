//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __GRAPH_HPP__
# define __GRAPH_HPP__

#include <iostream>
#include <vector>

#include "Vector2.hpp"
#include "Line.hpp"

namespace ex01_bonus {
    class Graph {
        public:
            Graph(const std::string& file);
            Graph(const Vector2& size);
            ~Graph();
            const Vector2& get_size() const;
            bool add_point(const Vector2& p);
            void add_line(const Line & l);
            void save_to_png(const std::string& filename, float scale = 1) const;
        private:
            Vector2 parse_file(const std::string& filename);
            bool has_point(const Vector2& p) const;
            Vector2 size_;
            std::vector<Vector2> points_;
            std::vector<Line> lines_;
    };
}
#endif
