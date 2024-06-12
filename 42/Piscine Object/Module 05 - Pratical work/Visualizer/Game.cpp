#include "Game.hpp"
#include "raylib.h"

Game::Game(Graph* graph)
    : graph_(graph),
      menu_state_(this),
      simulation_state_(this),
      network_state_(this),
      settings_state_(this),
      current_state_(&menu_state_) // Start with the menu state
{
    network_state_.SetGraph(graph_);
}

void Game::Run() {
    while (!WindowShouldClose()) {
        Update();
        Draw();
    }
}

void Game::ChangeState(IGameState* new_state) {
    current_state_ = new_state;
}

void Game::Update() {
    if (current_state_) {
        current_state_->Update();
    }
}

void Game::Draw() {
    BeginDrawing();
    ClearBackground(RAYWHITE);

    if (current_state_) {
        current_state_->Draw();
    }

    EndDrawing();
}

Graph* Game::GetGraph() const {
    return graph_;
}
