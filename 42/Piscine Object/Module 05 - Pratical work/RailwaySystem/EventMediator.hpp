//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __EVENTMEDIATOR_HPP__
# define __EVENTMEDIATOR_HPP__

#include <iostream>

class Simulation;

class EventMediator {
 public:
  EventMediator(Simulation & simulation);
  void UpdateEvents();
 private:
  Simulation& simulation_;
};
#endif
