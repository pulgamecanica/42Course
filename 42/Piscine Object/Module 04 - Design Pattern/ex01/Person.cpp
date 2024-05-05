//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "School.inc"

#include "Person.hpp"
#include "Room.hpp"

Person::Person(const std::string & name): name_(name), current_room_(nullptr) {
    if (DEBUG)
        std::cout << YELLOW << "[Person] " << GREEN << "CREATE\t" << *this << std::endl;
}

Person::~Person() {
    current_room_ = nullptr;
    if (DEBUG)
        std::cout << "Person " << *this << " destroyed" << std::endl;
}

void Person::setRoom(Room *p_room) {
    // If p_room is null, we are trying to set the room to nothing (exit the room where we are)
    // So we should not call exit, because it could cause issues with the print of the messages
    if (current_room_ != nullptr && p_room != nullptr) {
        if (DEBUG)
            std::cout << YELLOW << "[Person] " << GREEN << "SET ROOM[" << YELLOW << "exit prev room" << GREEN << "]\t" << ENDC << *current_room_ << " - " << *this << std::endl;
        current_room_->exit(this);
    }
    current_room_ = p_room;
    if (DEBUG && p_room == nullptr)
        std::cout << YELLOW << "[Person] " << GREEN << "SET ROOM\t" << ENDC << p_room << " - " << *this << std::endl;
    else if (DEBUG && p_room != nullptr)
        std::cout << YELLOW << "[Person] " << GREEN << "SET ROOM\t" << ENDC << *p_room << " - " << *this << std::endl;
}

const Room * Person::getRoom() const {
    return (current_room_);
}

const std::string & Person::getName() const {
    return (name_);
}

std::ostream& operator<<(std::ostream& s, const Person& p) {
    s << BLUE << p.getName() << ENDC;
    return (s);
}

