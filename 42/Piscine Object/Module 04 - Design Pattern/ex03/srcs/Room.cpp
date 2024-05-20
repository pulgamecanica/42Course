//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "Room.hpp"
#include "Person.hpp"

#include <algorithm>

#include "ex03.inc"

int Room::next_id_ = 0;

Room::Room() : id_(next_id_++) {
  if (DEBUG)
    std::cout << GREEN << "[NEW]" << YELLOW << "[Room]\t" << ENDC << id_ << std::endl;
}

Room::~Room() {
  if (DEBUG)
    std::cout << RED << "[DESTROY]" << YELLOW << "[Room]\t" << ENDC << id_ << std::endl;
}

void Room::Enter(std::shared_ptr<Person> person) {
  if (!CanEnter(person)) {
    std::cout << RED << "[" << YELLOW << "Warning" << RED << " ENTER]" << ENDC << *this << ", " << *person << " Cannot enter." << std::endl;
    return ;
  }
  for (const auto& occupant : occupants_) {
    if (occupant.lock() == person) {
      std::cout << RED << "[" << YELLOW << "Warning" << RED << " ENTER]" << ENDC << *this << "Cannot enter the same room where you are." << std::endl;
      return;
    }
  }
  occupants_.push_back(person);
  person->SetCurrentRoom(shared_from_this());
  if (DEBUG)
    std::cout << *this << GREEN << " [ENTER]\t" << ENDC << *person << std::endl;
}

void Room::Exit(std::shared_ptr<Person> person) {
  auto it = std::remove_if(occupants_.begin(), occupants_.end(),
                           [&person](const std::weak_ptr<Person>& p) {
                             return p.lock() == person;
                           });
  if (it == occupants_.end()) {
    std::cout << RED << "[" << YELLOW << "Warning" << RED << " EXIT]" << ENDC << *this << " Cannot exit a room where you are not" << std::endl;
  } else {
    occupants_.erase(it, occupants_.end());
    person->ClearCurrentRoom();
    if (DEBUG)
      std::cout << *this << GREEN << " [EXIT]\t" << ENDC << *person << std::endl;
  }
}

int Room::GetID() const {
  return id_;
}

void Room::PrintOccupant() const {
  for (auto p : occupants_)
    if (!p.expired())
      std::cout << *p.lock() << std::endl;
}

std::ostream& operator<<(std::ostream& s, const Room& room) {
  s << BLUE << "[Room] " << ENDC << room.GetID();
  return (s);
}
