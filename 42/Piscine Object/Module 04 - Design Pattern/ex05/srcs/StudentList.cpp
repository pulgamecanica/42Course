//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "StudentList.hpp"

#include "ex04.inc"

StudentList *StudentList::instance_{nullptr};

StudentList::StudentList(): Singleton("StudentList") {
  if (DEBUG)
    std::cout << GREEN << "[NEW]" << YELLOW << "[StudentList]\t" << ENDC << std::endl;
}

StudentList::~StudentList() {
  if (instance_ != nullptr)
    delete instance_;
  if (DEBUG)
    std::cout << RED << "[DESTROY]" << YELLOW << "[StudentList]\t" << ENDC << std::endl;
}

StudentList *StudentList::GetInstance() {
  std::lock_guard<std::mutex> lock(mutex_);
  if (instance_ == nullptr)
    instance_ = new StudentList();
  return instance_;
}

std::ostream& operator<<(std::ostream& s, const StudentList& sl) {
  s << BLUE << "[Singleton StudentList] : " << ENDC << sl.size();
  return (s);
}

