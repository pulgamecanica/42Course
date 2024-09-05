#ifndef SCHEDULE_HPP
#define SCHEDULE_HPP

#include <memory>
#include <string>
#include <vector>
#include "Train.hpp"

class Schedule {
 public:
  explicit Schedule(const std::string& name);
  
  void AddTrain(std::unique_ptr<Train>& train);
  void PrintTrainData() const;
  const std::vector<std::unique_ptr<Train>>& GetTrains() const;
  const std::string& GetName() const;

 private:
  const std::string name_;
  std::vector<std::unique_ptr<Train>> trains_;
};

#endif  // SCHEDULE_HPP
