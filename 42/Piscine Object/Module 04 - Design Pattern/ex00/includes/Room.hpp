//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __ROOM_HPP__
# define __ROOM_HPP__

#include <iostream>
#include <vector>
#include <memory>

class Person;

class Room: public std::enable_shared_from_this<Room> {
  public:
  Room();
  ~Room();
  Room(const Room&) = default;
  Room& operator= (const Room&) = default;
  void Enter(std::shared_ptr<Person> person);
  void Exit(std::shared_ptr<Person> person);
  int GetID() const;

  private:
  static int next_id_;
  
  protected:
  const int id_;
  std::vector<std::weak_ptr<Person>> occupants_;
};
std::ostream& operator<<(std::ostream&, const Room&);

#endif // __ROOM_HPP__
