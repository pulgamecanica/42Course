//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "Account.hpp"
#include "ex00.inc"

namespace ex00 {
	Account::Account(int id): id_(id), value_(0) {
	}

	Account::~Account() {
	}

	int Account::get_id() const {
		return id_;
	}

	int Account::get_value() const {
		return value_;
	}

	void Account::edit(int value) {
		value_ += value;
	}

	bool Account::operator==(int id) {
		return id == id_;
	}

	std::ostream& operator<<(std::ostream& s, const Account& a) {
		s << "[" << GREEN << a.get_id() << ENDC << "] - [" << GREEN << a.get_value() << ENDC << "]";
		return (s);
	}
}
