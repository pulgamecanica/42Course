#include "Schedule.hpp"
#include "Parser.hpp"
#include <iostream>

Schedule::Schedule(const std::string& name) : name_(name) {}

void Schedule::AddTrain(std::unique_ptr<Train>& train) {
  trains_.push_back(std::move(train));
}

const std::vector<std::unique_ptr<Train>>& Schedule::GetTrains() const {
  return trains_;
}

void Schedule::PrintTrainData() const {
  std::cout << "Schedule: " << name_ << std::endl;
  for (const auto& train : trains_) {
    std::string hour = Parser::ConvertToTimeString(train->GetHour());
    std::cout << "  Train: " << train->GetName() << ", MaxAcceleration: " << train->GetMaxAcceleration()
            << ", MaxBrakeForce: " << train->GetMaxBrakeForce() << ", Departure: " << train->GetDeparture()
            << ", Arrival: " << train->GetArrival() << ", Hour: " << hour << std::endl;
  }
}

const std::string& Schedule::GetName() const {
  return name_;
}
