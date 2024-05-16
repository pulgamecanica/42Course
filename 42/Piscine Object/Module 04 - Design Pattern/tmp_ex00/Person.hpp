//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __PERSON_HPP__
# define __PERSON_HPP__

#include <iostream>
#include <memory>

namespace SpringfieldElementary {

class Room;

class Person : public std::enable_shared_from_this<Person> {
public:
    Person(const std::string& p_name);
    virtual ~Person();
    const std::string& getName() const;
    std::shared_ptr<Room> getRoom() const;
    void setRoom(std::shared_ptr<Room> room);
private:
    std::string name_;
    std::shared_ptr<Room> current_room_;
};
std::ostream& operator<<(std::ostream &, const Person &);
}
#endif
