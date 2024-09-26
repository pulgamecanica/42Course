//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef TRAINSTIMEMEDIATOR_HPP__
# define TRAINSTIMEMEDIATOR_HPP__

#include "Subject.hpp"

#include <iostream>
#include <map>

class SimulationsManager;
class Train;

class TrainsTimeMediator: public Subject {
 public:
  TrainsTimeMediator(const SimulationsManager& manager);
  unsigned int GetTrainAverageTime(const Train* train) const;
  void SetupAverages();
 private:
  const SimulationsManager& manager_;
  std::map<const Train*, unsigned int> train_averages_;
};
#endif // TRAINSTIMEMEDIATOR_HPP__
