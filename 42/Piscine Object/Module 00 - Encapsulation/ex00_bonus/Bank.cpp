//***************************//
//*Template by pulgamecanica*//
//***************************//

#include <algorithm> // For find
#include <sstream> // For stringstream
#include <cmath> // for abs

#include "ex00_bonus.inc"
#include "Bank.hpp"

namespace ex00_bonus {
    int Bank::global_id = 0;

    Bank::Bank(const std::string &name, int liquidity): name_(name), liquidity_(0) {
        if (DEBUG)
            std::cout << "System\t" << GREEN << "CREATE\t" << ENDC << "Bank" << std::endl;
        edit_liquidity(liquidity);
    }

    Bank::~Bank() {
        if (DEBUG)
            std::cout << "System\t" << GREEN << "DESTROY\t" << ENDC << "Bank" << std::endl;
        wallets_.clear();
        client_accounts_.clear();
    }

    Bank::Wallet::Wallet():  value_(0) {
        ;
    }

    Bank::Wallet::~Wallet() {
        ;
    }

    float Bank::Wallet::get_value() const {
        return value_;
    }

    void Bank::Wallet::edit_value(float deposit) {
        value_ += deposit;
    }

    Bank::Account::Account(int id): id_(id) {
        ;
    }

    Bank::Account::~Account() {
        ;
    }

    int Bank::Account::get_id() const {
        return id_;
    }

    bool Bank::Account::operator==(const int id) const {
        return id_ == id;
    }

    bool Bank::Account::operator==(const int id) {
        return id_ == id;
    }

    bool Bank::Account::operator==(const Account& rhs) {
        return id_ == rhs.get_id();
    }

    Bank::Account::operator std::string() const {
        std::stringstream ss;
        ss << "[" << GREEN << get_id() << ENDC << "]" << ENDC;
        return (ss.str());
    }

    float Bank::get_account_value(int id) const {
        return wallets_.at(id).get_value();
    }

    int Bank::get_liquidity() const {
        if (DEBUG)
            std::cout << "Bank\t" << GREEN << "GET\t\t" << ENDC << "liquidity" << std::endl;
        return liquidity_;
    }

    const std::string & Bank::get_name() const {
        if (DEBUG)
            std::cout << "Bank\t" << GREEN << "GET\t\t" << ENDC << "name" << std::endl;
        return name_;
    }

    const std::string Bank::get_client_accounts_info() const {
        std::stringstream ss;
        if (DEBUG)
            std::cout << "Bank\t" << GREEN << "GET\t\t" << ENDC << "client accounts" << std::endl;
        for (
            std::vector<Account>::const_iterator client_account = client_accounts_.begin();
            client_account != client_accounts_.end();
            ++client_account)
            ss << "  " << std::string(*client_account) << " $" << get_account_value((*client_account).get_id()) << std::endl;
        return ss.str();
    }

    Bank::Account const &Bank::operator[](unsigned int id) const {
        std::stringstream ss;
        ss << "Account `" << id << "` was not founded";
        std::string errmsg = ss.str();
        std::vector<Bank::Account>::const_iterator it = std::find(client_accounts_.begin(), client_accounts_.end(), id);
        if (it == client_accounts_.end())
            throw std::out_of_range(errmsg);
        return *it;
    }

    Bank::Account &Bank::operator[](unsigned int id) {
        std::stringstream ss;
        ss << "Account `" << id << "` was not founded";
        std::string errmsg = ss.str();
        std::vector<Bank::Account>::iterator it = std::find(client_accounts_.begin(), client_accounts_.end(), id);
        if (it == client_accounts_.end())
            throw std::out_of_range(errmsg);
        return *it;
    }

    void Bank::edit_liquidity(int value) {
        if (value > 0 && DEBUG)
                std::cout << "Bank\t" << GREEN << "SET\t\t" << ENDC << "liquidity " << GREEN "$" << abs(value) << ENDC << std::endl;
        else if (value < 0 && DEBUG)
            std::cout << "Bank\t" << GREEN << "SET\t\t" << ENDC << "liquidity " << RED << "-$" << abs(value) << ENDC << std::endl;
        liquidity_ += value;
        if (liquidity_ < 0 && DEBUG)
                std::cout << "Bank\t" << GREEN << "BANKRUPTCY\t" << ENDC << std::endl;
    }

    int Bank::create_account() {
        Bank::Account new_account = Bank::Account(global_id);
        client_accounts_.push_back(new_account);
        if (DEBUG)
            std::cout << "Bank\t" << GREEN << "CREATE\t\t" << ENDC << "account:" << new_account.get_id() << std::endl;
        Bank::global_id++;
        std::find(client_accounts_.begin(), client_accounts_.end(), new_account.get_id());
        wallets_[new_account.get_id()] = Wallet();
        return new_account.get_id();
    }

    void Bank::delete_account(int id) {
        std::vector<Bank::Account>::iterator it = std::find(client_accounts_.begin(), client_accounts_.end(), id);
        if (it == client_accounts_.end())
            return ;
        if (DEBUG)
            std::cout << "Bank\t" << GREEN << "DESTROY\t\t" << ENDC << "account:" << id << std::endl;
        wallets_.erase(id);
        client_accounts_.erase(it);
    }

    void Bank::edit_account(int id, int amount) throw(std::out_of_range) {
        Bank::Account a = (*this)[id];
        float commision = abs(amount) * 0.05;
        int total = amount - commision;
        int value = get_account_value(id);
        if (amount > 0) { // Deposit money  
            if (value < 0) {
                int loan_returned = value + total;
                if (loan_returned >= 0) {
                    if (DEBUG)
                        std::cout << "Bank\t" << GREEN << "LOAN SOLVED\t" << ENDC << "account:" << id << GREEN " Thank you for begin a loyal member of " << BLUE << name_ << GREEN " bank" << ENDC << std::endl;
                    edit_liquidity(abs(value));
                } else {
                    if (DEBUG)
                        std::cout << "Bank\t" << GREEN << "PAY LOAN\t" << ENDC << "account:" << id << BLUE " $" << abs(total) << ENDC << std::endl;
                    edit_liquidity(abs(total));
                }
            }
            if (DEBUG)
                std::cout << "Bank\t" << GREEN << "DEPOSIT\t\t" << ENDC << "account:" << id << GREEN " +$" << total << ENDC << " ($" << commision << " -> bank)" << std::endl;
        } else if (amount < 0) { // Withdraw money if possible, may need a load
            int loan = value - abs(total);
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
        wallets_[id].edit_value(total);
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
