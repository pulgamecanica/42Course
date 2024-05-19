//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __PERSON_HPP__
# define __PERSON_HPP__

#include <iostream>
#include <memory>

class Room;

class Person {
  public:
  explicit Person(const std::string& name);
  virtual ~Person();
  Person(const Person&) = default;
  Person& operator= (const Person&) = default;
  const std::string & GetName() const;
  void SetCurrentRoom(std::shared_ptr<Room> room);
  void ClearCurrentRoom();
  
  protected:
  std::shared_ptr<Room> GetCurrentRoom();
  const std::string name_;
  std::weak_ptr<Room> current_room_;
};
std::ostream& operator<<(std::ostream&, const Person&);

#endif // __PERSON_HPP__
