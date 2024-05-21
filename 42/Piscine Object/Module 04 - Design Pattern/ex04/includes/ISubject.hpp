//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __ISUBJECT__
# define __ISUBJECT__

#include <memory>

// https://refactoring.guru/design-patterns/observer
// "Bell" is a a subject
// We could have more subjects to which Obserbers can subscribe
// In the future we could have "Lunch" time as well
// They observe the Event, which is controlled by the Headmaster
class IObserver;

class ISubject {
 public:
  virtual ~ISubject(){
    ;
  };
  virtual void Attach(std::shared_ptr<IObserver> observer) = 0;
  virtual void Detach(std::shared_ptr<IObserver> observer) = 0;
  virtual void Notify() = 0;
};

#endif