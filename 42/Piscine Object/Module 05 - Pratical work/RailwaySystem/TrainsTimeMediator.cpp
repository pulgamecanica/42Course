#include "TrainsTimeMediator.hpp"
#include "SimulationsManager.hpp"
#include "Train.hpp"

TrainsTimeMediator::TrainsTimeMediator(const SimulationsManager& manager)
  : manager_(manager) {

}

unsigned int TrainsTimeMediator::GetTrainAverageTime(const Train* train) const {
  return train_averages_.at(train);
}

void TrainsTimeMediator::SetupAverages() {
  for (const auto & obs : observers_) {
    TrainSimulation* ts = (TrainSimulation*)obs;
    if (!ts)
      throw std::runtime_error("Couldn't convert observer to Train Simulation");
    const Train* train = &ts->GetTrain(); // Carefull... I guess ??? Maybe ?
    train_averages_[train] += ts->GetTotalTime();

  }
  for (const auto &[train, avg] : train_averages_) {
    train_averages_[train] /= manager_.GetSimulations().size();
  }
}