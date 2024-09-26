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
      event_mediator_(*this),
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

std::vector<std::unique_ptr<NodeSimulation>>& Simulation::GetNodes() {
  return nodes_;
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
    LogSimulationState();
    HandleCollisions();
    HandleEvents();
    if (HasFinished()) {
      LogSimulationState();
      state_ = State::kFinished;
    }
  }
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

unsigned int Simulation::GetStartTime() const {
  return start_time_;
}

void Simulation::HandleEvents() {
  event_mediator_.UpdateEvents();
}

void Simulation::HandleCollisions() {
  collision_mediator_.CheckForCollisions();
}

void Simulation::LogSimulationState() {
  std::vector<std::shared_ptr<TrainSimulationState>> states_;
  std::vector<std::shared_ptr<EventSimulationState>> events_;
  for (const auto& train : trains_) {
    TrainSimulationState state = train->GetCurrentState();
    states_.push_back(std::make_shared<TrainSimulationState>(state));
  }
  for (const auto& node : nodes_) {
    for (const auto& event : node->GetEventsOccurrences()) {
      EventSimulationState state = event->GetCurrentState();
      events_.push_back(std::make_shared<EventSimulationState>(state));
    }
  }
  trains_states_.push_back(states_);
  events_states_.push_back(events_);
}

const std::vector<std::shared_ptr<TrainSimulationState>>& Simulation::GetSimulationTrainsState(int index) {
  return trains_states_[index];
}
const std::vector<std::shared_ptr<EventSimulationState>>& Simulation::GetSimulationEventsState(int index) {
  return events_states_[index];
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

void Simulation::Log() const {
  for (const auto& train : trains_) {
    FileLogger logger(GetDirectory() + "/" + train->GetTrain().GetName() + ".log");
    logger.write(std::string("Train : ") + train->GetTrain().GetName());
    logger.write(std::string("Estimated optimal travel time : ") + Parser::ConvertToTimeStringHHMMSS(train->GetOptimalTime()));
    logger.write(std::string("Travel from ") + train->GetTrain().GetDeparture() + " to " + train->GetTrain().GetArrival() + " at " + Parser::ConvertToTimeStringHHMMSS(train->GetTrain().GetHour()));
    logger.write("");
    logger.write(train->GetLogs());
  }
}
