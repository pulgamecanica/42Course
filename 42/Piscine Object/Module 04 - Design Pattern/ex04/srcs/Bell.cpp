//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "Bell.hpp"
#include "IObserver.hpp"

#include <algorithm>

#include "ex04.inc"

Bell::Bell() {
  if (DEBUG)
    std::cout << GREEN << "[NEW]" << YELLOW << "[BELL]\t" << ENDC << std::endl;
}

Bell::~Bell() {
  if (DEBUG)
    std::cout << RED << "[DESTROY]" << YELLOW << "[BELL]\t" << ENDC << std::endl;
}

void Bell::Attach(std::shared_ptr<IObserver> observer) {
  observers_.push_back(observer);
  if (DEBUG)
    std::cout << *this << GREEN << " [ATACH]\t" << ENDC << std::endl;
}

void Bell::Detach(std::shared_ptr<IObserver> observer) {
  auto it = std::remove_if(observers_.begin(), observers_.end(),
                           [&observer](const std::weak_ptr<IObserver>& o) {
                             return o.lock() == observer;
                           });
  if (it == observers_.end()) {
    std::cout << RED << "[" << YELLOW << "Warning" << RED << " DETACH]" << ENDC << *this << "Removing an unexisting observer." << std::endl;
  } else {
    observers_.erase(it, observers_.end());
    if (DEBUG)
      std::cout << *this << GREEN << " [DETACH]\t" << ENDC << std::endl;
  }
}

void Bell::Notify() {
  for (auto obsrv: observers_)
    if (!obsrv.expired())
      obsrv.lock()->Update(Event::RingBell);
  std::cout << *this << GREEN << "  [NOTIFY]\t" << ENDC << std::endl;
}

std::ostream& operator<<(std::ostream& s, const Bell& param) {
  (void)param;
  s << BLUE << "[BELL]";
  return (s);
}

