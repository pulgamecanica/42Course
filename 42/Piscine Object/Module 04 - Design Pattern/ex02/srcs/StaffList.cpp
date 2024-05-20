//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "StaffList.hpp"

#include "ex02.inc"

StaffList *StaffList::instance_{nullptr};

StaffList::StaffList(): Singleton("StaffList") {
  if (DEBUG)
    std::cout << GREEN << "[NEW]" << YELLOW << "[StaffList]\t" << ENDC << std::endl;
}

StaffList::~StaffList() {
  if (instance_ != nullptr)
    delete instance_;
  if (DEBUG)
    std::cout << RED << "[DESTROY]" << YELLOW << "[StaffList]\t" << ENDC << std::endl;
}

StaffList *StaffList::GetInstance() {
  std::lock_guard<std::mutex> lock(mutex_);
  if (instance_ == nullptr)
    instance_ = new StaffList();
  return instance_;
}

std::ostream& operator<<(std::ostream& s, const StaffList& sl) {
  s << BLUE << "[Singleton StaffList] : " << ENDC << sl.size();
  return (s);
}

