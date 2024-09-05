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

// TrainSimulation Implementation
TrainSimulation::TrainSimulation(Simulation& simulation, const Train& train)
  : simulation_(simulation),
    train_(train),
    node_source_(*simulation.GetNode(train.GetDeparture())),
    node_destiny_(*simulation.GetNode(train.GetArrival())),
    logger_(simulation.GetDirectory() + "/" + train.GetName() + "_train.log"),
    current_rail_(nullptr),
    current_node_(simulation.GetNode(train.GetDeparture())),
    prev_node_name_(train.GetDeparture()),
    position_m_(0.0f),
    speed_(0.0f),
    acceleration_(0.0f),
    status_(Status::kStoped),
    has_safe_distance_(true),
    event_warning_stop_(false),
    time_running_s_(train.GetHour()) {

    CalculateFastestRoute();
    total_distance_ = path_info_.TotalDistance();
    optimal_time_ = 0;
    if (total_distance_ != -1) {
      for(auto const &[node, dist] : path_info_.path) {
        optimal_time_ += GetOptimalTimeForDistance(dist);
      }
    }
    // std::cout << "Optimal time for train: " << train_.GetName() << " [" << GetOptimalTime() << "s]" << "(" << total_distance_ << "m)" << std::endl;
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
      // UpdateStatus();
    } else {
      ManageArrivalToNode();
      CalculateFastestRoute();
      if (CanStart())
        StartRoute();
    }
    UpdateStatus();
    Log();
  }
  time_running_s_++;
}

void TrainSimulation::Update(Subject* subject) {
  NodeSimulation * node = (NodeSimulation*)subject;
  RailSimulation * rail = (RailSimulation*)subject;
  if (node) {
    ; // something
  } else if (rail) {
    ; // something
  }
}

void TrainSimulation::Log() {
  if (HasFinished()) return ;
  std::string status_str_list[4] = {"Stoped", "Speed Up", "Braking", "Mantaining"}; 
  std::string status_str = status_str_list[status_];
  std::stringstream ss;

  if (HasArrivedToNode()) {
    ss << "[" << Parser::ConvertToTimeString(time_running_s_) << "] - [" << node_source_.GetNode().GetName() << "][" << node_destiny_.GetNode().GetName() << "] - [Waiting]";
    // std::cout << "[Status: " << status_str << "] - [Dist: " << position_m_ << "m/" << current_rail_->GetRail().GetDistance() << "m] - (Speed: " << speed_ << "m/s) - (Acceleration: " << acceleration_ << "N) - {Stoping distance: " << GetStoppingDistance() << "}" << std::endl;
  } else {
    // Check for other trains at the same rail
    double distance_left = current_rail_->GetRail().GetDistance() - position_m_;
    if (distance_left < 0)
      distance_left = 0;
    const std::string train_str_rep = GetRailStringRep();
    ss << "[" << Parser::ConvertToTimeString(time_running_s_) << "] - [" << node_source_.GetNode().GetName() << "][" << node_destiny_.GetNode().GetName() << "] - [" << distance_left << "km] - [" << status_str << "] - " << train_str_rep;
    ss << " (" << current_rail_->GetObservers().size() << ")";
  }
  // std::cout << ss.str() << std::endl;
  logger_.write(ss.str());
  // Create the Simulation Tree to explore train path later
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
  if (HasStoped()) return;
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

bool TrainSimulation::ShouldStop() const {
  // if STOP_SIGNAL_MAYDAY
  //   return true
  if (!current_rail_)
    throw std::runtime_error("Should be on a rail to call `ShouldStop`");
  float distance_left = current_rail_->GetRail().GetDistance() - position_m_;
  return (!has_safe_distance_ || distance_left <= GetStoppingDistance());
}

bool TrainSimulation::HasArrivedToNode() const {
  if (current_rail_ == nullptr && current_node_ != nullptr) return true;
  if (current_rail_ != nullptr && TraveledAllRail()) return true;
  return false;
}

bool TrainSimulation::CanStart() const {
  return !(event_warning_stop_ || !IsTimeToStart() || HasFinished());
}

bool TrainSimulation::IsTimeToStart() const {
  return time_running_s_ >= train_.GetHour();
}

bool TrainSimulation::TraveledAllRail() const {
  if (!current_rail_)
    throw std::runtime_error("Cannot calculate traveled rail due to: missing rail");
  return current_rail_->GetRail().GetDistance() <= position_m_ && HasStoped();
}

double TrainSimulation::GetMaxAccelerationForce() const {
  return train_.GetMaxAcceleration();
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