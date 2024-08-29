#include "Simulation/TrainSimulation.hpp"
#include "Simulation/RailSimulation.hpp"
#include "Simulation/Simulation.hpp"
#include "Settings.hpp"
#include <algorithm>
#include <cmath>

TrainSimulation::TrainSimulation(Simulation& simulation, const Train& train)
  : simulation_(simulation),
    train_(train),
    node_source_(simulation.GetNode(train.departure)),
    node_destiny_(simulation.GetNode(train.arrival)),
    current_rail_(nullptr),
    current_node_(&(simulation.GetNode(train.departure))),
    position_m_(0.0f),
    speed_(0.0f),
    acceleration_(0.0f),
    status_(Status::STOPED),
    has_safe_distance_(true) {

    CalculateFastestRoute();
    total_distance_ = path_info_.TotalDistance();
    optimal_time_ = 0;
    if (total_distance_ != -1) {
      for(auto const [node, dist] : path_info_.path) {
        optimal_time_ += GetOptimalTimeForDistance(dist);
      }
    }
    std::cout << "Optimal time for train: " << GetName() << " [" << GetOptimalTime() << "s]" << "(" << total_distance_ << "m)" << std::endl;
}

std::string TrainSimulation::GetName() const {
  return train_.name;
}

// Each frame is equivalent to 1 second
void TrainSimulation::Update() {
  if (!HasFinished()) {
    if (!HasArrivedToNode()) {
      if (ShouldStop() && !HasStoped()) {
        Brake();
      } else {
        Accelerate();
      }
      UpdatePosition();
      UpdateStatus();
    } else {
      ManageArrivalToNode();
      CalculateFastestRoute();
      if (CanStart()) {
        StartRoute();
      }
    }
  }
  std::string status_str = "";
  if (status_ == STOPED)
    status_str = "STOPED";
  else if (status_ == ACCELERATING)
    status_str = "ACCELERATING";
  else if (status_ == BRAKING)
    status_str = "BRAKING";
  else if (status_ == MANTAINING)
    status_str = "MANTAINING";
  if (HasFinished())
    std::cout << "[FINISHED]" << std::endl;
  else if (!HasArrivedToNode())
    std::cout << "[Status: " << status_str << "] - [Dist: " << position_m_ << "m/" << current_rail_->Distance() << "m] - (Speed: " << speed_ << "m/s) - (Acceleration: " << acceleration_ << "N) - {Stoping distance: " << CalculateStoppingDistance() << "}" << std::endl;
  time_passed_s_++;
}

void TrainSimulation::StartRoute() {
  // current_rail_ == // Setup rail
  if (HasFinished()) return;
  if (path_info_.path.size() < 2) return;
  auto it = path_info_.path.begin();

  const std::string node_start = it->first.GetKey();
  it++;
  const std::string node_finish = it->first.GetKey();
  RailSimulation* rail = simulation_.GetRailRef(node_start, node_finish);
  if (rail == nullptr)
    throw std::runtime_error("Rail not founded! Check the Path to make sure nodes are connected");
  // if (rail available to destiny) -> set rail
  //  Subscribe to rail
  //  Unsibscribe from node
  next_node_name_ = std::string(node_finish);
  current_rail_ = rail;
  current_node_ = nullptr;
  speed_ = 0;
  position_m_ = 0;
  acceleration_ = 0;
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
    status_ = Status::ACCELERATING;
  } else if (HasStoped()) {
    status_ = Status::STOPED;
    acceleration_ = 0;
  } else if (acceleration_ < 0) {
    status_ = Status::BRAKING;
  } else {
    status_ = Status::MANTAINING;
  }
}

void TrainSimulation::ManageArrivalToNode() {
  if (current_rail_ == nullptr && current_node_ != nullptr) return;
  std::cout << "Arrived to " << next_node_name_ << std::endl;
  total_distance_ += current_rail_->Distance();
  current_node_ = &simulation_.GetNode(next_node_name_);
  current_rail_ = nullptr;
  next_node_name_ = "";
}

bool TrainSimulation::ShouldStop() const {
  // if STOP_SIGNAL_MAYDAY
  //   return true
  if (!current_rail_)
    throw std::runtime_error("Should be on a rail to call `ShouldStop`");
  float distance_left = current_rail_->Distance() - position_m_;
  return (!has_safe_distance_ || distance_left <= CalculateStoppingDistance());
}

// void TrainSimulation::StartMoving() {
//   if (current_rail_ || HasFinished()) return;
//   if (current_node_ || !current_rail_) {
//     // RailSimulation& next_rail = NULL;
//     // RailSimulation& next_rail = simulation_.GetRail(shortest_path_.NextRail(current_rail_));
//     // SubscribeRail(next_rail);
//     position_m_ = 0;
//   }
//   status_ = Status::ACCELERATING;
// }


bool TrainSimulation::HasArrivedToNode() const {
  if (current_rail_ == nullptr && current_node_ != nullptr) return true;
  if (current_rail_ != nullptr && TraveledAllRail()) return true;
  return false;
}

bool TrainSimulation::CanStart() const {
  // if event not true
  // if departure time is ok
  if (HasFinished()) return false;
  return true;
}

void TrainSimulation::SubscribeNode(NodeSimulation& node) {
  UnsubscribeCurrentRail();
  current_node_ = &node;
  node.AddTrain(this);
}

void TrainSimulation::SubscribeRail(RailSimulation& rail) {
  UnsubscribeCurrentNode();
  current_rail_ = &rail;
  rail.AddTrain(this);
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

bool TrainSimulation::TraveledAllRail() const {
  if (!current_rail_)
    throw std::runtime_error("Cannot calculate traveled rail due to: missing rail");
  return current_rail_->Distance() <= position_m_ && HasStoped();
}

double TrainSimulation::GetMaxAccelerationForce() const {
  return train_.max_acceleration;
}

double TrainSimulation::GetMaxBrakeForce() const {
  return train_.max_brake_force;
}

const std::string TrainSimulation::GetArrivalName() const {
  return train_.arrival;
}

const std::string TrainSimulation::GetDepartureName() const {
  return train_.departure;
}

bool TrainSimulation::HasStoped() const {
  return speed_ == 0;
}

bool TrainSimulation::HasFinished() const {
  if (current_node_ == nullptr) return false;
  if (InvalidPath())  return true;
  return node_destiny_.GetName() == current_node_->GetName();
}

bool  TrainSimulation::InvalidPath() const {
  return total_distance_ == -1;
}

void TrainSimulation::Update(Subject* subject) {
  if (auto* rail = dynamic_cast<RailSimulation*>(subject)) {
    // React to rail updates (e.g., potential collision)
    // Activate alerts
    // has_safe_distance_ = false
    // Mediator is in charge of alerting the subject on the colisions and events
  }
}

void TrainSimulation::SetRail(RailSimulation* rail) {
  current_rail_ = rail;
}

void TrainSimulation::SetNode(NodeSimulation* node) {
  current_node_ = node;
}

void TrainSimulation::CalculateFastestRoute() {
  // Implement route calculation logic
  path_info_ = simulation_.GetRailwaySystem().GetGraph().Dijkstra(current_node_->GetName(), node_destiny_.GetName());
}

float TrainSimulation::CalculateStoppingDistance() const {
  // Simple formula for stopping distance (assuming constant deceleration)
  return (speed_ * speed_) / (2 * GetMaxBrakeForce());
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

double TrainSimulation::TimePassedS() const {
  return time_passed_s_;
}
