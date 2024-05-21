//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __BELL_HPP__
# define __BELL_HPP__

#include "ISubject.hpp"

#include <iostream>
#include <vector>
#include <memory>

class IObserver;

class Bell: public ISubject {
  public:
  Bell();
  ~Bell();
  Bell(const Bell&) = delete;
  Bell& operator= (const Bell&) = delete;
  void Attach(std::shared_ptr<IObserver> observer);
  void Detach(std::shared_ptr<IObserver> observer);
  void Notify();
  
  private:
  std::vector<std::weak_ptr<IObserver>> observers_;
};
std::ostream& operator<<(std::ostream&, const Bell&);
#endif
