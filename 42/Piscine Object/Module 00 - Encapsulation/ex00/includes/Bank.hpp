//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __BANK_HPP__
# define __BANK_HPP__

#include <iostream>
#include <vector>

#include "Account.hpp"

namespace ex00 {
	class Bank {
		public:
			static int global_id;
			Bank(const std::string &, int liquidity = 0);
			~Bank();
			// Const methods
			int get_liquidity() const;
			const std::string & get_name() const;
			const std::string get_client_accounts_info() const;
			// methods
			void edit_liquidity(int value);
			int create_account();
			void delete_account(int id);
			void edit_account(int id, int amount);
			// friend methods
			friend std::ostream&	operator<<(std::ostream&, const Bank&);
		private:
			std::vector<Account *>::iterator get_account(int id);
			const std::string name_;
			int liquidity_;
			std::vector<Account *> client_accounts_;
	};
}
#endif
