#ifndef GAME_HPP
#define GAME_HPP

#include "Graph.hpp"
#include "IGameState.hpp"
#include "MenuState.hpp"
#include "SimulationState.hpp"
#include "NetworkState.hpp"
#include "SettingsState.hpp"
#include <memory>

class Game {
public:
    Game(Graph* graph);
    void Run();
    void ChangeState(IGameState* new_state);

    Graph* GetGraph() const;

    // Persistent state instances
    MenuState menu_state_;
    SimulationState simulation_state_;
    NetworkState network_state_;
    SettingsState settings_state_;

private:
    void Update();
    void Draw();

    IGameState* current_state_;
    Graph* graph_;
};

#endif // GAME_HPP
