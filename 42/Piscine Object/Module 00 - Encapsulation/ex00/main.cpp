//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "ex00.inc"
#include "Bank.hpp"
#include "Account.hpp"

int	main(void)
{
	std::cout << YELLOW << "- - - - - - INIT - - - - - - -" << ENDC << std::endl;
	ex00::Bank bank("Morgan Stanley", 100);
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
	bank.edit_account(2, +100);
	bank.edit_account(2, +21);
	bank.edit_account(3, +42);
	bank.edit_account(3, +42);
	std::cout << bank << std::endl;
	std::cout << YELLOW << "- - - - - - - EDIT WITHDRAW - - - - - -" << ENDC << std::endl;
	bank.edit_account(3, -42);
	bank.edit_account(2, -88);
	std::cout << bank << std::endl;
	std::cout << YELLOW << "- - - - - - - EDIT ERROR - - - - - -" << ENDC << std::endl;
	bank.edit_account(42, -142);
	bank.edit_account(1, -142);
	bank.edit_account(0, +142);
	bank.edit_account(142, +142);
	std::cout << YELLOW << "- - - - - - - LOAN - - - - - -" << ENDC << std::endl;
	bank.create_account();
	bank.edit_account(4, +42);
	bank.edit_account(4, -42);
	bank.edit_account(4, +200);
	bank.edit_account(4, -4200);
	std::cout << bank << std::endl;
	std::cout << YELLOW << "- - - - - - - - - - - - -" << ENDC << std::endl;
	return (0);
}
