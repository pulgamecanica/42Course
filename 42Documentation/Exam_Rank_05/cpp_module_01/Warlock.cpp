#include "Warlock.hpp"

Warlock::Warlock(const std::string & name, const std::string & title): _name(name), _title(title) {};
Warlock::~Warlock() {};
void Warlock::setTitle(const std::string & name) {_name = name;};
const std::string & Warlock::getName() const {return (_name);};
const std::string & Warlock::getTitle() const {return (_title);};


int main(void) {
	Warlock bob("Bob", "the magnificent");
	std::cout << bob.getName() << std::endl;
	bob.setTitle("Weirdo");
	std::cout << bob.getName() << std::endl;
};

