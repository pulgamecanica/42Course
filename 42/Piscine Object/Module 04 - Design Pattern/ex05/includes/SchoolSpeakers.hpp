//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __SCHOOL_SPEAKERS_HPP__
# define __SCHOOL_SPEAKERS_HPP__

#include "ISubject.hpp"

#include <iostream>
#include <vector>
#include <memory>

class IObserver;

class SchoolSpeakers: public ISubject {
  public:
  SchoolSpeakers();
  ~SchoolSpeakers();
  SchoolSpeakers(const SchoolSpeakers&) = default;
  SchoolSpeakers& operator= (const SchoolSpeakers&) = default;
  void Attach(std::shared_ptr<IObserver> observer);
  void Detach(std::shared_ptr<IObserver> observer);
  void Notify(enum Event e);
  
  private:
  std::vector<std::weak_ptr<IObserver>> observers_;
};
std::ostream& operator<<(std::ostream&, const SchoolSpeakers&);
#endif
