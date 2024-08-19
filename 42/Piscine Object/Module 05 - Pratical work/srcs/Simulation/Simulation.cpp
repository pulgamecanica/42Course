#include "Simulation/Parser.hpp"
#include "Simulation/Simulation.hpp"
#include "Settings.hpp"

Simulation::Simulation(const RailwaySystem &rail_sys, const Schedule & schedule)
  : rail_sys_(rail_sys),
    schedule_(schedule),
    logger_filename_(Settings::Instance().GetOutputDirectory() + "/" + schedule.GetName() + "_" + Parser::ParseCurrentTimeString() + ".log"),
    logger_(logger_filename_) {
  InitializeNodes();
  InitializeRails();
  InitializeTrains();
}

void Simulation::InitializeNodes() {
  for (const auto &[name, node] : rail_sys_.GetNodes())
    nodes_.emplace_back(node, &event_mediator_);
}

void Simulation::InitializeRails() {
  for (const auto &rail : rail_sys_.GetRails())
    rails_.emplace_back(rail, &collision_mediator_);
}

void Simulation::InitializeTrains() {
  for (const auto &train : schedule_.GetTrains())
    trains_.emplace_back(train);
}

void Simulation::Update() {
  for (auto & train : trains_)
    train.Update();
  HandleEvents();
  HandleCollisions();
  LogSimulationState();
}

void Simulation::HandleEvents() {
  // Logic to process events
}

void Simulation::HandleCollisions() {
  // Logic to process collisions
}

void Simulation::LogSimulationState() {
  // Logic to log the current state of the simulation
}
