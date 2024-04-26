//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __ACCOUNT_HPP__
# define __ACCOUNT_HPP__

#include <iostream>

namespace ex00 {
	class Account {
		public:
			Account(int id);
			~Account();
			bool operator==(int id);
			int get_id() const;
			int get_value() const;
			void edit(int value);
			friend std::ostream&	operator<<(std::ostream&, const Account&);
		private:
			int id_;
			int value_;
	};
}
#endif
