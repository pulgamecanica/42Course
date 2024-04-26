//***************************//
//*Template by pulgamecanica*//
//***************************//

#include <vector>

#include "ex01_bonus.inc"

#include "lodepng.h"

#include "Vector2.hpp"
#include "Line.hpp"
#include "Graph.hpp"


static void usage() {
    std::cout << "Usage: ex01_bonus input_file output_file" << std::endl;
    std::cout << "  input_file: Path to the input ASCII art file" << std::endl;
    std::cout << "  output_file: Path to save the output PNG image" << std::endl;
}

int main(int argc, char *argv[]) {
    std::string file("");
    std::string outfile("output.png");
    ex01_bonus::Graph * g;

    if (argc > 3) {
        usage();
        return 1;
    }
    if(argc >= 3)
        outfile = std::string(argv[2]);
    if(argc >= 2) {
        try {
            g = new ex01_bonus::Graph(std::string(argv[1]));
        } catch (std::exception &e) {
            std::cerr << "Error while creating Graph" << std::endl;
            return 1;
        }
    } else {
        g = new ex01_bonus::Graph(ex01_bonus::Vector2(42, 42));
    
        ex01_bonus::Vector2 p1(0, 0);
        ex01_bonus::Vector2 p2(22, 12);
        ex01_bonus::Vector2 p3(21, 21);
        ex01_bonus::Vector2 p4(40, 9);
        ex01_bonus::Vector2 p5(9, 22);
        ex01_bonus::Vector2 p6(9, 40);
        
        g->add_point(p1);
        g->add_point(p2);
        g->add_point(p3);
        g->add_point(p4);
        g->add_point(p5);
        g->add_point(p6);

        uint32_t color = ex01_bonus::gen_trgb(255, 24, 200, 42);
        ex01_bonus::Line l1(p1, p6, color);
        g->add_line(l1);
    }
    g->save_to_png(outfile, 1);
    
    delete g;
}
