#ifndef NAVBAR_HPP
#define NAVBAR_HPP

#include "raylib.h"

class Game;

class Navbar {
public:
    Navbar(bool* show_grid, float* zoom, float* node_size);
    void Draw();
private:
    bool* show_grid_;
    float* zoom_;
    float* node_size_;
    Game* game_;
};

#endif // NAVBAR_HPP
