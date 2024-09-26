#include "Train.hpp"
#include "Rail.hpp"
#include "Simulation.hpp"
#include "Parser.hpp"
#include <algorithm>
#include <cmath>
#include <sstream>

// Train Implementation
Train::Train(const std::string& name, double maxAcceleration, double maxBrakeForce, 
             const std::string& departure, const std::string& arrival, unsigned hour)
    : name_(name), 
      departure_(departure), 
      arrival_(arrival), 
      maxAcceleration_(maxAcceleration), 
      maxBrakeForce_(maxBrakeForce), 
      departure_hour_s_(hour) {}

const std::string& Train::GetName() const {
  return name_;
}

double Train::GetMaxAcceleration() const {
  return maxAcceleration_;
}

double Train::GetMaxBrakeForce() const {
  return maxBrakeForce_;
}

const std::string& Train::GetDeparture() const {
  return departure_;
}

const std::string& Train::GetArrival() const {
  return arrival_;
}

unsigned Train::GetHour() const {
  return departure_hour_s_;
}

// TrainSimulationState Implementation
TrainSimulationState::TrainSimulationState(
  const TrainSimulation* train_simulation,
  const RailSimulation* current_rail,
  const NodeSimulation* current_node,
  const std::string& next_node_name,
  const std::string& prev_node_name,
  float position_m,
  float speed,
  float acceleration,
  bool safe_distance,
  bool event_warning_stop,
  double total_distance)
  : train_simulation_(train_simulation),
    current_rail_(current_rail),
    current_node_(current_node),
    next_node_name_(next_node_name),
    prev_node_name_(prev_node_name),
    position_m_(position_m),
    speed_(speed),
    acceleration_(acceleration),
    safe_distance_(safe_distance),
    event_warning_stop_(event_warning_stop),
    total_distance_(total_distance) {
    if (!train_simulation_)
      throw std::runtime_error("Train Simulation must not be null");
}

const std::string TrainSimulationState::GetCurrentPositionName() const {
  if (!HasArrivedToNode())
    return GetPrevNodeName() + " - " + GetNextNodeName();
  return prev_node_name_;
}

const RailSimulation* TrainSimulationState::GetCurrentRail() const {
  return current_rail_;
}

const NodeSimulation* TrainSimulationState::GetCurrentNode() const {
  return current_node_;
}

const std::string& TrainSimulationState::GetNextNodeName() const {
  return next_node_name_;
}

const std::string& TrainSimulationState::GetPrevNodeName() const {
  return prev_node_name_;
}

bool TrainSimulationState::HasArrivedToNode() const {
  return current_rail_ == nullptr;
}

bool TrainSimulationState::HasSafeDistance() const {
  return safe_distance_;
}

bool TrainSimulationState::HasEventWarningStop() const {
  return event_warning_stop_;
}

float TrainSimulationState::GetPosition() const {
  return position_m_;
}

float TrainSimulationState::GetSpeed() const {
  return speed_;
}

float TrainSimulationState::GetAcceleration() const {
  return acceleration_;
}

double TrainSimulationState::GetTotalDistance() const {
  return total_distance_;
}

const TrainSimulation* TrainSimulationState::GetTrainSimulation() const {
  return train_simulation_;
}

const std::string& TrainSimulationState::GetArrival() const {
  return train_simulation_->GetTrain().GetArrival();
}

const std::string& TrainSimulationState::GetDeparture() const {
  return train_simulation_->GetTrain().GetDeparture();
}

const std::string& TrainSimulationState::GetName() const {
  return train_simulation_->GetTrain().GetName();
}

const std::string TrainSimulationState::GetHour() const {
  return Parser::ConvertToTimeString(train_simulation_->GetTrain().GetHour());
}

// TrainSimulation Implementation
TrainSimulation::TrainSimulation(Simulation& simulation, const Train& train)
  : simulation_(simulation),
    train_(train),
    node_source_(*simulation.GetNode(train.GetDeparture())),
    node_destiny_(*simulation.GetNode(train.GetArrival())),
    // logger_(simulation.GetDirectory() + "/" + train.GetName() + "_train.log"),
    current_rail_(nullptr),
    current_node_(simulation.GetNode(train.GetDeparture())),
    prev_node_name_(train.GetDeparture()),
    position_m_(0.0f),
    speed_(0.0f),
    acceleration_(0.0f),
    status_(Status::kStoped),
    event_warning_stop_(false),
    train_can_start_(true),
    has_safe_distance_(true),
    time_running_s_(0) {

    SetMaxAcceleration(train_.GetMaxAcceleration());
    CalculateFastestRoute();
    total_distance_ = path_info_.TotalDistance();
    optimal_time_ = 0;
    unsigned last_dist = 0;
    if (total_distance_ != -1) {
      for(auto const &[node, dist] : path_info_.path) {
        optimal_time_ += GetOptimalTimeForDistance(dist - last_dist);
        last_dist = dist;
      }
    }
}

// // Each frame is equivalent to 1 second
void TrainSimulation::Update() {
  if (!HasFinished()) {
    if (!HasArrivedToNode()) {
      if (ShouldStop() && !HasStoped())
        Brake();
      else
        Accelerate();
      UpdatePosition();
      Log();
    } else {
      if (train_can_start_) {
        if (CanStart())
          StartRoute();
      } else {
        ManageArrivalToNode();
        CalculateFastestRoute();
      }
      Log();
    }
    UpdateStatus();
    time_running_s_++;
  }
}

unsigned int TrainSimulation::GetTotalTime() const {
  return time_running_s_;
}

void TrainSimulation::Update(Subject* subject) {
  // Collision notification
  RailSimulation* rail_subject = (RailSimulation*)subject;
  NodeSimulation* node_subject = (NodeSimulation*)subject;
  if (rail_subject) {
    has_safe_distance_ = false;
  } else if (node_subject) {
    //; Nothing really
  }
}

void TrainSimulation::Log() {
  if (HasFinished()) return ;
  std::string status_str_list[4] = {"Stoped", "Speed Up", "Braking", "Mantaining"}; 
  std::string status_str = status_str_list[status_];
  std::stringstream ss;

  if (HasArrivedToNode()) {
    if (current_node_)
      ss << "[" << Parser::ConvertToTimeString(simulation_.GetCurrentTime()) << "] - [" << current_node_->GetNode().GetName() << "] - [Waiting]";
  } else {
    double distance_left = current_rail_->GetRail().GetDistance() - position_m_;
    if (distance_left < 0)
      distance_left = 0;
    const std::string train_str_rep = GetRailStringRep();
    ss << "[" << Parser::ConvertToTimeString(simulation_.GetCurrentTime()) << "] - [" << prev_node_name_ << "][" << next_node_name_ << "] - [" << distance_left / 1000 << "km | " << speed_ << "m/s] - [" << status_str << "] - " << train_str_rep;
    ss << " (" << current_rail_->GetObservers().size() << ")";
  }
  if (!ss.str().empty()) {
    ss << std::endl;
    train_logs_.append(ss.str());
  }
}

const std::string& TrainSimulation::GetLogs() const {
  return train_logs_;
}


const std::string TrainSimulation::GetRailStringRep() const {
  if (!current_rail_) return std::string("");

  std::vector<TrainSimulation *> neighbor_trains;
  
  for (const auto & observer : current_rail_->GetObservers()) {
    TrainSimulation * train = (TrainSimulation *)observer;
    if (train == this) continue;
    if (!train)
      throw std::runtime_error("Observer couldn't be casted to train");
    neighbor_trains.push_back(train);
  }

  const int RAIL_LEN = 7;
  std::string rail_symbols[RAIL_LEN];
  for (int i = 0; i < RAIL_LEN; ++i)
    rail_symbols[i] = " ";
  for (const auto& neighbor : neighbor_trains) {
    std::string c = "O";
    if (simulation_.IsRailTwoWay()) {
      if (GetNextNodeDestiny() == neighbor->GetNextNodeDestiny()) {
        std::string c = ">";
      } else {
        std::string c = "<";
        // Inverse pos
      }
    }
    int pos = neighbor->GetPositionPercentage() * (RAIL_LEN - 1);
    if (pos >= RAIL_LEN)
      throw std::runtime_error("position cannot be larger than Rail lenght");
    rail_symbols[pos] = c;
  }
  int train_pos = GetPositionPercentage() * (RAIL_LEN - 1);
  if (train_pos >= RAIL_LEN)
      throw std::runtime_error("position cannot be larger than Rail lenght");
  rail_symbols[train_pos] = "x";
  // If it's two ways, The train Direction would be [<] or [>] instead of [X]
  std::string train_str_rep;
  for (int i = 0; i < RAIL_LEN; ++i) {
    if ((int)(RAIL_LEN / 2) == i && rail_symbols[i] == " ")
      rail_symbols[i] = "...";
    train_str_rep.append(std::string("[") + rail_symbols[i]  + std::string("]"));
  }
  return train_str_rep;
}

const Train & TrainSimulation::GetTrain() const {
  return train_;
}

const std::string & TrainSimulation::GetNextNodeDestiny() const {
  return next_node_name_;
}

void TrainSimulation::Accelerate() {
  if (!CanStart()) return;
  acceleration_ = GetMaxAccelerationForce();
}

void TrainSimulation::Brake() {
  if (HasStoped()) { return; }
  acceleration_ = -GetMaxBrakeForce();
}

void TrainSimulation::UpdatePosition() {
  double time_passed_s = 1; // Assuming this returns time in seconds, each frame is one second
  speed_ += time_passed_s * acceleration_;
  speed_ = std::max<float>(0, speed_); // max because the train cannot go in reverse
  if (speed_ >= simulation_.GetMaxTrainSpeed()) {
    speed_ = simulation_.GetMaxTrainSpeed(); // Manual limit, cannot go over the speed limit
    acceleration_ = 0;
  }
  position_m_ += speed_ * time_passed_s;
}

void TrainSimulation::UpdateStatus() {
  if (acceleration_ > 0) {
    status_ = Status::kAccelerating;
  } else if (HasStoped()) {
    status_ = Status::kStoped;
    acceleration_ = 0;
  } else if (acceleration_ < 0) {
    status_ = Status::kBraking;
  } else {
    status_ = Status::kMantaining;
  }
}

void TrainSimulation::ManageArrivalToNode() {
  if (current_rail_ == nullptr && current_node_ != nullptr) return; // Already managed

  NodeSimulation * node = simulation_.GetNode(next_node_name_);
  
  SubscribeToNode(node);

  prev_node_name_ = std::string(next_node_name_);
  next_node_name_ = "";
  position_m_ = 0;
  train_can_start_ = true;
}

void TrainSimulation::StartRoute() {
  if (HasFinished()) return;
  if (path_info_.path.size() < 2) return; // If the path is less than two, there is no route (no more than 2 nodes)
  auto it = path_info_.path.begin();

  const std::string node_start = it->first;
  it++;
  const std::string node_finish = it->first;
  RailSimulation* rail = simulation_.GetRailRef(node_start, node_finish);
  if (rail == nullptr)
    throw std::runtime_error("Rail not founded! Check the Path to make sure nodes are connected");
  
  // Check if can join the rail
  if (!simulation_.GetCollisionMediator().CanJoinRail(*rail, node_finish, simulation_.IsRailTwoWay())) return ;

  SubscribeToRail(rail);

  next_node_name_ = std::string(node_finish);
  speed_ = 0;
  position_m_ = 0;
  acceleration_ = 0;
  train_can_start_ = false;
}

void TrainSimulation::SubscribeToNode(NodeSimulation* node) {
  UnsubscribeCurrentRail();
  current_node_ = node;
  node->AddTrain(this);
}

void TrainSimulation::SubscribeToRail(RailSimulation* rail) {
  UnsubscribeCurrentNode();
  current_rail_ = rail;
  rail->AddTrain(this);
}

void TrainSimulation::UnsubscribeCurrentNode() {
  if (current_node_) {
    current_node_->RemoveTrain(this);
    current_node_ = nullptr;
  }
}

void TrainSimulation::UnsubscribeCurrentRail() {
  if (current_rail_) {
    current_rail_->RemoveTrain(this);
    current_rail_ = nullptr;
  }
}

bool TrainSimulation::ShouldStop() {
  if (!current_rail_)
    throw std::runtime_error("Should be on a rail to call `ShouldStop`");
  if (!HasSafeDistance())
    return true;
  float distance_left = current_rail_->GetRail().GetDistance() - position_m_;
  return (distance_left <= GetStoppingDistance());
}

bool TrainSimulation::HasSafeDistance() {
  if (HasStoped())
    has_safe_distance_ = true;
  return has_safe_distance_;
  // return simulation_.GetCollisionMediator().HasSafeDistance(current_rail_, this);
}

bool TrainSimulation::HasArrivedToNode() const {
  if (!IsTimeToStart()) return true;
  if (current_rail_ == nullptr && current_node_ != nullptr) return true;
  if (current_rail_ != nullptr && TraveledAllRail()) return true;
  return false;
}

bool TrainSimulation::CanStart() const {
  if (current_node_ && current_node_->IsNodeBlocked())
    return false;
  return !(event_warning_stop_ || !IsTimeToStart() || HasFinished());
}

bool TrainSimulation::IsTimeToStart() const {
  return simulation_.GetCurrentTime() >= train_.GetHour();
}

bool TrainSimulation::TraveledAllRail() const {
  if (!current_rail_)
    throw std::runtime_error("Cannot calculate traveled rail due to: missing rail");
  return current_rail_->GetRail().GetDistance() <= position_m_ && HasStoped();
}

void TrainSimulation::SetMaxAcceleration(double acceleration) {
  max_acceleration_ = acceleration; 
}

double TrainSimulation::GetMaxAccelerationForce() const {
  return max_acceleration_;
}

double TrainSimulation::GetMaxBrakeForce() const {
  return train_.GetMaxBrakeForce();
}

bool TrainSimulation::HasStoped() const {
  return speed_ == 0;
}

bool TrainSimulation::HasFinished() const {
  if (current_node_ == nullptr) return false;
  if (InvalidPath())  return true;
  return node_destiny_.GetNode().GetName() == current_node_->GetNode().GetName();
}

bool  TrainSimulation::InvalidPath() const {
  return total_distance_ == -1;
}

unsigned int TrainSimulation::GetCurrentTime() const {
  return simulation_.GetCurrentTime();
}


void TrainSimulation::CalculateFastestRoute() {
  // Do not calculate fastest route if it's finished
  if (node_destiny_.GetNode().GetName() != current_node_->GetNode().GetName())
    path_info_ = simulation_.GetRailwaySystem().GetGraph().Dijkstra(current_node_->GetNode().GetName(), node_destiny_.GetNode().GetName());
}

float TrainSimulation::GetStoppingDistance() const {
  // Simple formula for stopping distance (assuming constant deceleration)
  return (speed_ * speed_) / (2 * GetMaxBrakeForce());
}

float TrainSimulation::GetPosition() const {
  return position_m_;
}

float TrainSimulation::GetPositionPercentage() const {
  if (!current_rail_) return 0;
  float percentage = position_m_ / current_rail_->GetRail().GetDistance();
  if (percentage > 1)
    percentage = 1;
  return percentage;
}

double TrainSimulation::GetOptimalTimeForDistance(double distance) const {
  // Calculate the distances needed to accelerate to max speed and to decelerate
  double max_speed = simulation_.GetMaxTrainSpeed();
  double acceleration = GetMaxAccelerationForce();
  double deceleration = GetMaxBrakeForce();


  // Calculate the peak speed that can be reached given the distance
  double peak_speed = std::sqrt(2 * distance / ((1 / acceleration) + (1 / deceleration)));
  peak_speed = std::min(max_speed, peak_speed); // Cap peak speed if higher than max speed

  // Calculate time for acceleration and deceleration to this peak speed
  double t_acceleration = peak_speed / acceleration;
  double t_deceleration = peak_speed / deceleration;

  // If peak speed is less than max speed, calculate constant speed phase
  double t_constant_speed = 0.0;
  if (peak_speed == max_speed) {
    double distance_acceleration = (max_speed * max_speed) / (2 * acceleration);
    double distance_deceleration = (max_speed * max_speed) / (2 * deceleration);
    double remaining_distance = distance - (distance_acceleration + distance_deceleration);
    t_constant_speed = remaining_distance / max_speed;
  }

  return t_acceleration + t_constant_speed + t_deceleration;
}

double TrainSimulation::GetOptimalTime() const {
  return optimal_time_;
}

TrainSimulationState TrainSimulation::GetCurrentState() const {
  return TrainSimulationState(
    this,
    current_rail_,
    current_node_,
    next_node_name_,
    prev_node_name_,
    position_m_,
    speed_,
    acceleration_,
    has_safe_distance_,
    event_warning_stop_,
    total_distance_
  );
}
