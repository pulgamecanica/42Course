//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "Person.hpp"
#include "Room.hpp"

#include "ex01.inc"

Person::Person(const std::string & name): name_(name){
  if (DEBUG)
    std::cout << GREEN << "[NEW]" << YELLOW << "[Person]\t" << ENDC << name_ << std::endl;
}

Person::~Person() {
  if (DEBUG)
    std::cout << RED << "[DESTROY]" << YELLOW << "[Person]\t" << ENDC << name_ << std::endl;
}

const std::string & Person::GetName() const {
  return name_;
}

void Person::SetCurrentRoom(std::shared_ptr<Room> room) {
  if (current_room_.lock() == room) {
    std::cout << RED << "[" << YELLOW << "Warning" << RED << " SET CURRENT ROOM]" << ENDC << *this << " Cannot enter the same room where you are." << std::endl;
  } else {
    current_room_ = room;
    if (DEBUG) 
      std::cout << *this << GREEN << " [SET CURRENT ROOM]\t" << ENDC << *room << std::endl;
  }
}

void Person::ClearCurrentRoom() {
  if (current_room_.expired()) { // If you are not in a room
    std::cout << RED << "[" << YELLOW << "Warning" << RED << " SET CURRENT ROOM]" << ENDC << *this << " Cannot exit a room where you are not." << std::endl;
  } else {
    current_room_.reset();
    if (DEBUG)
      std::cout << *this << GREEN << " [CLEAR CURRENT ROOM]\t" << ENDC << std::endl;
  }
}

std::shared_ptr<Room> Person::GetCurrentRoom() {
  if (!current_room_.expired())
    return current_room_.lock();
  return nullptr;
}

std::ostream& operator<<(std::ostream& s, const Person& p) {
  s << BLUE << "[Person] " << ENDC << p.GetName();
  return (s);
}

