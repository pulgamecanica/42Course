//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __ISUBJECT__
# define __ISUBJECT__

#include "IObserver.hpp"

#include <memory>

// https://refactoring.guru/design-patterns/observer

class ISubject {
 public:
  virtual ~ISubject(){
    ;
  };
  virtual void Attach(std::shared_ptr<IObserver> observer) = 0;
  virtual void Detach(std::shared_ptr<IObserver> observer) = 0;
  virtual void Notify(enum Event e) = 0;
};

#endif