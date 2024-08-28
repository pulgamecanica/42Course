#include "Simulation/Parser.hpp"
#include "Simulation/Simulation.hpp"
#include "Settings.hpp"

Simulation::Simulation(const RailwaySystem &rail_sys, const Schedule & schedule)
  : rail_sys_(rail_sys),
    schedule_(schedule),
    logger_filename_(Settings::Instance().GetOutputDirectory() + "/" + schedule.GetName() + "_" + Parser::ParseCurrentTimeString() + ".log"),
    logger_(logger_filename_) {
  state_ = State::Starting;
  InitializeNodes();
  InitializeRails();
  InitializeTrains();
  state_ = State::Running;
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
    trains_.emplace_back(*this, train);
}

void Simulation::Update() {
  // for (auto & train : trains_)
  //   train.Update();
  HandleEvents();
  HandleCollisions();
  // LogSimulationState();
}

bool Simulation::IsFinished() const {
  return state_ == State::Finished;
}

double Simulation::GetRealTravelTime() const {
  if (state_ == State::Finished)
    return real_travel_time_;
  return -1;
};

double Simulation::GetOptimalTravelTime() const {
  double optimal_travel_time = 0;
  for (auto & train : trains_)
    optimal_travel_time += train.GetOptimalTime();
  return optimal_travel_time;
};

NodeSimulation & Simulation::GetNode(const std::string & node_name) {
  for (auto & node : nodes_)
    if (node.GetName() == node_name)
      return node;
  throw std::runtime_error("Node name not found");
}


// std::vector<std::string> Simulation::GetEventList() {
//   return std::vector<std::to_string>();
// }

void Simulation::HandleEvents() {
  // Logic to process events
}

void Simulation::HandleCollisions() {
  // Logic to process collisions
}

void Simulation::LogSimulationState() {
  // Logic to log the current state of the simulation
}
