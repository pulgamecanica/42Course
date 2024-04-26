//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "ex00_bonus.inc"
#include "Bank.hpp"

int main(void)
{
    if (DEBUG)
        std::cout << "Debug ON!" << std::endl;
    std::cout << YELLOW << "- - - - - - INIT - - - - - - -" << ENDC << std::endl;
    ex00_bonus::Bank bank("Morgan Stanley", 100);
    bank.create_account();
    bank.create_account();
    bank.create_account();
    std::cout << bank << std::endl;
    std::cout << YELLOW << "- - - - - - DELETE - - - - - - -" << ENDC << std::endl;
    bank.delete_account(0);
    bank.delete_account(42);
    bank.delete_account(4002);
    bank.delete_account(1);
    bank.create_account();
    std::cout << bank << std::endl;
    std::cout << YELLOW << "- - - - - - - EDIT DEPOSIT - - - - - -" << ENDC << std::endl;
    try {
        bank.edit_account(2, +100);
        bank.edit_account(2, +21);
        bank.edit_account(3, +42);
        bank.edit_account(3, +42);
    } catch (std::out_of_range & e) {
        std::cout << "Exception: " << RED << e.what() << ENDC << std::endl;
    }
    std::cout << bank << std::endl;
    std::cout << YELLOW << "- - - - - - - EDIT WITHDRAW - - - - - -" << ENDC << std::endl;
    try {
        bank.edit_account(3, -42);
        bank.edit_account(2, -88);
    } catch (std::out_of_range & e) {
        std::cout << "Exception: " << RED << e.what() << ENDC << std::endl;
    }
    std::cout << bank << std::endl;
    std::cout << YELLOW << "- - - - - - - EDIT ERROR - - - - - -" << ENDC << std::endl;
    try {
        bank.edit_account(42, -142);
    } catch (std::out_of_range & e) {
        std::cout << "Exception: " << RED << e.what() << ENDC << std::endl;
    }
    try {
        bank.edit_account(1, -142);
    } catch (std::out_of_range & e) {
        std::cout << "Exception: " << RED << e.what() << ENDC << std::endl;
    }
    try {
        bank.edit_account(0, +142);
    } catch (std::out_of_range & e) {
        std::cout << "Exception: " << RED << e.what() << ENDC << std::endl;
    }
    try {
        bank.edit_account(142, +142);
    } catch (std::out_of_range & e) {
        std::cout << "Exception: " << RED << e.what() << ENDC << std::endl;
    }
    std::cout << bank << std::endl;
    std::cout << YELLOW << "- - - - - - - LOAN 1 - - - - - -" << ENDC << std::endl;
        bank.create_account();
    try {
    bank.edit_account(4, +42);
    } catch (std::out_of_range & e) {
        std::cout << "Exception: " << RED << e.what() << ENDC << std::endl;
    }
    try {
        bank.edit_account(4, -42);
    } catch (std::out_of_range & e) {
        std::cout << "Exception: " << RED << e.what() << ENDC << std::endl;
    }
    std::cout << bank << std::endl;
    std::cout << YELLOW << "- - - - - - - LOAN 2 - - - - - -" << ENDC << std::endl;
    try {
        bank.edit_account(4, +200);
    } catch (std::out_of_range & e) {
        std::cout << "Exception: " << RED << e.what() << ENDC << std::endl;
    }
    try {
        bank.edit_account(4, -4200);
    } catch (std::out_of_range & e) {
        std::cout << "Exception: " << RED << e.what() << ENDC << std::endl;
    }
    std::cout << bank << std::endl;
    std::cout << YELLOW << "- - - - - - - LOAN 3 - - - - - -" << ENDC << std::endl;
    try {
        bank.edit_account(4, -250);
    } catch (std::out_of_range & e) {
        std::cout << "Exception: " << RED << e.what() << ENDC << std::endl;
    }
    std::cout << bank << std::endl;
    try {
        bank.edit_account(4, +20);
    } catch (std::out_of_range & e) {
        std::cout << "Exception: " << RED << e.what() << ENDC << std::endl;
    }
    std::cout << bank << std::endl;
    std::cout << YELLOW << "- - - - - - - Access - - - - - -" << ENDC << std::endl;
    try {
        ex00_bonus::Bank::Account ba = bank[4];
        std::cout << "Account" << std::string(ba) << " Wallet: " << bank.get_account_value(ba.get_id()) << std::endl;
        std::cout << std::string(bank[2]) << std::endl;
        std::cout << bank.get_account_value(bank[2].get_id()) << std::endl;
    } catch (std::out_of_range & e) {
        std::cout << "Exception: " << RED << e.what() << ENDC << std::endl;
    }
    std::cout << YELLOW << "- - - - - - - - - - - - - - - - -" << ENDC << std::endl;
    return (0);
}
