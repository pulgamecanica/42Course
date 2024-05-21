//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __PERSON_HPP__
# define __PERSON_HPP__

#include <iostream>
#include <memory>

class Room;
class IMediator;

class Person: public std::enable_shared_from_this<Person> {
  public:
  explicit Person(const std::string& name, std::shared_ptr<IMediator> mediator = nullptr);
  virtual ~Person();
  Person(const Person&) = default;
  Person& operator= (const Person&) = default;
  const std::string & GetName() const;
  void SetCurrentRoom(std::shared_ptr<Room> room);
  void ClearCurrentRoom();
  void SetMediator(std::shared_ptr<IMediator> mediator);
  std::shared_ptr<Room> GetCurrentRoom() const;

  protected:
  const std::string name_;
  std::weak_ptr<Room> current_room_;
  std::shared_ptr<IMediator> mediator_;
};
std::ostream& operator<<(std::ostream&, const Person&);

#endif // __PERSON_HPP__
