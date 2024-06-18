#include "Navbar.hpp"
#include "Game.hpp"
#include "raygui.h"

Navbar::Navbar(bool* show_grid, float* zoom, float* node_size) : show_grid_(show_grid), zoom_(zoom), node_size_(node_size) {}

void Navbar::Draw() {
    Rectangle navbar = {0, 0, GetScreenWidth(), 40};
    // GuiPanel(navbar);

    if (GuiButton((Rectangle){10, 10, 100, 20}, "Menu")) {
        game_->ChangeState(&game_->menu_state_);
    }

    if (show_grid_) {
        GuiCheckBox((Rectangle){120, 10, 20, 20}, "Show Grid", show_grid_);
    }
    if (zoom_) {
        GuiSlider((Rectangle){225, 10, 100, 20}, "Zoom -", "Zoom +", zoom_, 0.15f, 4.0f);
    }
    if (node_size_) {
        GuiSlider((Rectangle){450, 10, 100, 20}, "Node Size -", "Node Size +", node_size_, 1.0f, 25.0f);
    }
}