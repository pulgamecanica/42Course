//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "RoomList.hpp"

#include "ex01.inc"

RoomList *RoomList::instance_{nullptr};

RoomList::RoomList(): Singleton("RoomList") {
  if (DEBUG)
    std::cout << GREEN << "[NEW]" << YELLOW << "[RoomList]\t" << ENDC << std::endl;
}

RoomList::~RoomList() {
  if (instance_ != nullptr)
    delete instance_;
  if (DEBUG)
    std::cout << RED << "[DESTROY]" << YELLOW << "[RoomList]\t" << ENDC << std::endl;
}


RoomList *RoomList::GetInstance() {
  std::lock_guard<std::mutex> lock(mutex_);
  if (instance_ == nullptr)
    instance_ = new RoomList();
  return instance_;
}

std::ostream& operator<<(std::ostream& s, const RoomList& rl) {
  s << BLUE << "[Singleton RoomList] : " << ENDC << rl.size();
  return (s);
}

