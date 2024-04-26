//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __BANK_HPP__
# define __BANK_HPP__

#include <iostream>
#include <vector>
#include <map>

namespace ex00_bonus {
    class Bank {
        public:
            class Account {
                public:
                    Account(int id);
                    ~Account();
                    bool operator==(const int id) const;
                    bool operator==(const int id);
                    bool operator==(const Account& rhs);
                    int get_id() const;
                    operator std::string() const;
                private:
                    int id_;
            };
            Bank(const std::string &name, int liquidity);
            ~Bank();
            // const methods
            float get_account_value(int id) const;
            int get_liquidity() const;
            const std::string &get_name() const;
            const std::string get_client_accounts_info() const;
            Account const &operator[](unsigned int i) const;
            // // methods
            Account &operator[](unsigned int i);
            void edit_liquidity(int value);
            int create_account();
            void delete_account(int id);
            void edit_account(int id, int amount) throw(std::out_of_range);
            // // friend methods
            friend std::ostream&    operator<<(std::ostream&, const Bank&);
        private:
            static int global_id;
            class Wallet {
                public:
                    Wallet();
                    ~Wallet();
                    float get_value() const;
                    void edit_value(float deposit);
                private:
                    float value_;
            };
            const std::string name_;
            int liquidity_;
            std::vector<Account> client_accounts_;
            std::map<int, Wallet> wallets_;
    };
}
#endif
