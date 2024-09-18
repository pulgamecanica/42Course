#include "Simulation.hpp"
#include "Parser.hpp"
#include "Settings.hpp"

Simulation::Simulation(const RailwaySystem& railSys, const Schedule& schedule, int id)
    : rail_sys_(railSys),
      schedule_(schedule),
      id_(id),
      rail_two_way_(Settings::Instance().IsRailTwoWay()),
      directory_(Settings::Instance().GetOutputDirectory() + "/" +
                      schedule.GetName() + "_" + std::to_string(id) + "_" + Parser::ParseCurrentTimeString()),
      logger_(directory_ + "/simulation.log"),
      collision_mediator_(*this),
      state_(State::kStarting),
      max_speed_(Settings::Instance().MaxTrainSpeed()),
      total_time_(0) {
  InitializeNodes();
  InitializeRails();
  InitializeTrains();

  start_time_ = trains_.front()->GetTrain().GetHour();
  for (const auto & train : trains_)
    start_time_ = std::min(start_time_, train->GetTrain().GetHour());

  logger_.write(std::string("Simulation #") + std::to_string(id_));
  logger_.write(std::string("Total Trains: ") + std::to_string(trains_.size()));
  logger_.write(std::string("Total Nodes: ") + std::to_string(nodes_.size()));
  logger_.write(std::string("Total Rails: ") + std::to_string(rails_.size()));

  // This should be done when collecting results
  logger_.write("Total Event Counter: TODO");
  logger_.write("Time Spent Simulating: TODO");
  logger_.write("");
  for(const auto & train : trains_)
    logger_.write("Train " + train->GetTrain().GetName() + " | 15:00 - 22:00 | Real Time [39m] | Optimal Time [" + std::to_string(train->GetOptimalTime()) + "]");
  logger_.write("");
  logger_.write("Status: OK/KO");
  state_ = State::kRunning;
}

void Simulation::InitializeNodes() {
  for (const auto& [name, node] : rail_sys_.GetNodes()) {
    nodes_.emplace_back(std::make_unique<NodeSimulation>(*node, &event_mediator_));
  }
}

void Simulation::InitializeRails() {
  for (const auto& rail : rail_sys_.GetRails()) {
    rails_.emplace_back(std::make_unique<RailSimulation>(*rail, &collision_mediator_));
  }
}

void Simulation::InitializeTrains() {
  for (const auto& train : schedule_.GetTrains()) {
    trains_.emplace_back(std::make_unique<TrainSimulation>(*this, *train));
  }
}

std::vector<std::unique_ptr<RailSimulation>>& Simulation::GetRails() {
  return rails_;
}

std::vector<std::unique_ptr<TrainSimulation>>& Simulation::GetTrains() {
  return trains_;
}


const CollisionMediator& Simulation::GetCollisionMediator() const {
  return collision_mediator_;
}

NodeSimulation* Simulation::GetNode(const std::string& nodeName) {
  for (auto& node_s : nodes_) {
    if (node_s->GetNode().GetName() == nodeName) {
      return node_s.get();
    }
  }
  throw std::runtime_error("Node name not found");
}

void Simulation::Update() {
  if (state_ == State::kRunning) {
    total_time_++;
    for (auto& train : trains_)
      train->Update();
    // HandleEvents();
    // HandleCollisions();
    LogSimulationState();
    if (HasFinished()) {
      CollectResults();
      state_ = State::kFinished;
    }
  }
}

void Simulation::CollectResults() {
  
  // std::cout << "Finished Simulation " << schedule_.GetName() << std::endl;
}

bool Simulation::HasFinished() const {
  for (const auto& train : trains_)
    if (!train->HasFinished())
      return false;
  return true;
}

bool Simulation::IsFinished() const {
  return state_ == State::kFinished;
}

// double Simulation::GetRealTravelTime() const {
//   if (state_ == State::kFinished) {
//     return real_travel_time_;
//   }
//   return -1;
// }

// double Simulation::GetOptimalTravelTime() const {
//   double optimalTravelTime = 0;
//   for (const auto& train : trains_) {
//     optimalTravelTime += train->GetOptimalTime();
//   }
//   return optimalTravelTime;
// }

RailSimulation* Simulation::GetRailRef(const std::string& node1, const std::string& node2) {
  for (auto& rail : rails_) {
    if (rail->HasNodes(node1, node2)) {
      return rail.get();
    }
  }
  return nullptr;
}

const RailwaySystem& Simulation::GetRailwaySystem() const {
  return rail_sys_;
}

// void Simulation::HandleEvents() {
//   // Logic to process events
// }

// void Simulation::HandleCollisions() {
//   // Logic to process collisions
// }

void Simulation::LogSimulationState() {
  std::vector<std::shared_ptr<TrainSimulationState>> states_;
  for (auto& train : trains_) {
    TrainSimulationState state = train->GetCurrentState();
    states_.push_back(std::make_shared<TrainSimulationState>(state));
  }
  trains_states_.push_back(states_);
}

const std::vector<std::shared_ptr<TrainSimulationState>>& Simulation::GetSimulationState(int index) {
  return trains_states_[index];
}

bool Simulation::IsRailTwoWay() const {
  return rail_two_way_;
}

const std::string& Simulation::GetDirectory() const {
  return directory_;
}

double Simulation::GetMaxTrainSpeed() const {
  return max_speed_;
}

unsigned int Simulation::GetCurrentTime() const {
  return start_time_ + total_time_;
}

unsigned int Simulation::GetTotalTime() const {
  return total_time_;
}