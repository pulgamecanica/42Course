//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "EventMediator.hpp"
#include "Simulation.hpp"

EventMediator::EventMediator(Simulation & simulation)
  : simulation_(simulation) {}

void EventMediator::UpdateEvents() {
  for (auto & node : simulation_.GetNodes()) {
    for (auto & event : node->GetEventsOccurrences()) {
      if (!event->IsFinished() && simulation_.GetCurrentTime() >= event->GetFinishTime())
        event->SetFinished();
    }
  }
}

