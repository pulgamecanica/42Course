//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "CourseList.hpp"

#include "ex02.inc"

CourseList *CourseList::instance_{nullptr};

CourseList::CourseList(): Singleton("CourseList") {
  if (DEBUG)
    std::cout << GREEN << "[NEW]" << YELLOW << "[CourseList]\t" << ENDC << std::endl;
}

CourseList::~CourseList() {
  if (instance_ != nullptr)
    delete instance_;
  if (DEBUG)
    std::cout << RED << "[DESTROY]" << YELLOW << "[CourseList]\t" << ENDC << std::endl;
}

CourseList *CourseList::GetInstance() {
  std::lock_guard<std::mutex> lock(mutex_);
  if (instance_ == nullptr)
    instance_ = new CourseList();
  return instance_;
}

std::ostream& operator<<(std::ostream& s, const CourseList& cl) {
  s << BLUE << "[Singleton CourseList] : " << ENDC << cl.size();
  return (s);
}

