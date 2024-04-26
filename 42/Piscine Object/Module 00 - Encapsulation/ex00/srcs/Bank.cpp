//***************************//
//*Template by pulgamecanica*//
//***************************//

#include <algorithm>
#include <sstream>
#include <cmath>

#include "Bank.hpp"
#include "ex00.inc"

namespace ex00 {
	int Bank::global_id = 0;

	Bank::Bank(const std::string & name, int liquidity): name_(name), liquidity_(0) {
		if (DEBUG)
			std::cout << "System\t" << GREEN << "CREATE\t" << ENDC << "Bank" << std::endl;
		edit_liquidity(liquidity);
	}

	Bank::~Bank() {
		if (DEBUG)
			std::cout << "System\t" << GREEN << "DESTROY\t" << ENDC << "Bank" << std::endl;
		for (
			std::vector<Account *>::const_iterator clientAccount = client_accounts_.begin();
			clientAccount != client_accounts_.end();
			++clientAccount)
			delete *clientAccount;
		client_accounts_.clear();
	}

	int Bank::get_liquidity() const {
		if (DEBUG)
			std::cout << "Bank\t" << GREEN << "GET\t" << ENDC << "liquidity" << std::endl;
		return liquidity_;
	}

	const std::string & Bank::get_name() const {
		if (DEBUG)
			std::cout << "Bank\t" << GREEN << "GET\t" << ENDC << "name" << std::endl;
		return name_;
	}

	const std::string Bank::get_client_accounts_info() const {
		std::stringstream ss;
		if (DEBUG)
			std::cout << "Bank\t" << GREEN << "GET\t" << ENDC << "client accounts" << std::endl;
		for (
			std::vector<Account *>::const_iterator clientAccount = client_accounts_.begin();
			clientAccount != client_accounts_.end();
			++clientAccount)
			ss << "  " << *(*clientAccount) << std::endl;
		return ss.str();
	}

	std::vector<Account *>::iterator Bank::get_account(int id) {
		for (
			std::vector<Account *>::iterator clientAccount = client_accounts_.begin();
			clientAccount != client_accounts_.end();
			++clientAccount) {
			if (*(*clientAccount) == id) {
				if (DEBUG)
					std::cout << "Bank\t" << GREEN << "GET\t" << ENDC << "account:" << id << std::endl;
				return clientAccount;
			}
		}
		if (DEBUG)
			std::cout << "Bank\t" << GREEN << "GET\t" << RED << "account not found:" << id << ENDC << std::endl;
		return client_accounts_.end();
	}

	void Bank::edit_liquidity(int value) {
		if (value > 0 && DEBUG)
				std::cout << "Bank\t" << GREEN << "SET\t" << ENDC << "liquidity " << GREEN "$" << abs(value) << ENDC << std::endl;
		else if (value < 0 && DEBUG)
			std::cout << "Bank\t" << GREEN << "SET\t" << ENDC << "liquidity " << RED << "-$" << abs(value) << ENDC << std::endl;
		liquidity_ += value;
		if (liquidity_ < 0 && DEBUG)
				std::cout << "Bank\t" << GREEN << "BANKRUPTCY\t" << ENDC << std::endl;
	}


	int Bank::create_account() {
		Account * new_account = new Account(global_id);
		client_accounts_.push_back(new_account);
		if (DEBUG)
			std::cout << "Bank\t" << GREEN << "CREATE\t" << ENDC << "account:" << new_account->get_id() << std::endl;
		Bank::global_id++;
		return new_account->get_id();
	}

	void Bank::delete_account(int id) {
		std::vector<Account *>::iterator it = get_account(id);
		if (it == client_accounts_.end())
			return ;
		if (DEBUG)
			std::cout << "Bank\t" << GREEN << "DESTROY\t" << ENDC << "account:" << id << std::endl;
		delete *it;
		client_accounts_.erase(it);
	}

	void Bank::edit_account(int id, int amount) {
		std::vector<Account *>::iterator it = get_account(id);
		if (it == client_accounts_.end())
			return ;
		float commision = abs(amount) * 0.05;
		int total = amount - commision;
		if (amount > 0) { // Deposit money	
			if ((*(it))->get_value() < 0) {
				int loan_returned = (*(it))->get_value() + total;
				if (loan_returned >= 0) {
					if (DEBUG)
						std::cout << "Bank\t" << GREEN << "LOAN SOLVED\t" << ENDC << "account:" << id << GREEN " Thank you for begin a loyal member of " << BLUE << name_ << GREEN " bank" << ENDC << std::endl;
					edit_liquidity(abs((*(it))->get_value()));
				}
				else {
					if (DEBUG)
						std::cout << "Bank\t" << GREEN << "PAY LOAN\t" << ENDC << "account:" << id << GREEN " $" << loan_returned << ENDC << std::endl;
					edit_liquidity(abs(total));
				}
			}
			if (DEBUG)
				std::cout << "Bank\t" << GREEN << "DEPOSIT\t" << ENDC << "account:" << id << GREEN " +$" << total << ENDC << " ($" << commision << " -> bank)" << std::endl;
		} else if (amount < 0) { // Withdraw money if possible, may need a load
			int loan = (*(it))->get_value() - abs(total);
			if (loan < 0) {
				if (abs(loan) > (int)liquidity_) {
					std::cout << "Bank\t" << GREEN << "LOAN DENIED\t" << ENDC << "account:" << id << BLUE << " " << name_  << ENDC << " bank can't loan that amount " << "'$" << abs(loan) << "'" << std::endl;
					return;
				}
				if (DEBUG)
					std::cout << "Bank\t" << GREEN << "TAKE LOAN\t" << ENDC << "account:" << id << BLUE " $" << abs(loan) << ENDC << std::endl;
				edit_liquidity(loan);
			}
			if (DEBUG)
				std::cout << "Bank\t" << GREEN << "WITHDRAW\t" << ENDC << "account:" << id << RED " -$" << abs(total) << ENDC << " ($" << commision << " -> bank)" << std::endl;
		}
		(*(it))->edit(total);
	}

	std::ostream& operator<<(std::ostream& s, const Bank& p_bank) {
		int liquidity = p_bank.get_liquidity();
		const std::string & name = p_bank.get_name(); 
		const std::string accounts_info = p_bank.get_client_accounts_info();

		s << "Bank informations : " << BLUE << name << ENDC << std::endl;
		s << "Liquidity : " << GREEN << liquidity << ENDC << std::endl;
		s << "Clients:" << std::endl;
		s << accounts_info;
		return (s);
	}
}
