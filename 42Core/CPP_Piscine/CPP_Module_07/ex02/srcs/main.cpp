//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "Array.hpp"

#define MAX_VAL 750
int	main(void)
{
	Array<int> a1(5);
	a1[0] = 2;
	a1[1] = 4242;
	a1[2] = 42;
	a1[3] = 24;
	a1[4] = 4;
	std::cout << a1;
	Array<std::string> a2(3);
	a2[0] = "Hello";
	a2[1] = "Friend";
	a2[2] = "Good Bye";
	std::cout << a2;
	try {
		a2[3] = "WRONG...";
		std::cout << a2;
	}catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;
	}
	try {
		a2[-1] = "WRONG...";
		std::cout << a2;
	}catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;
	}
	std::cout << "size a1: (" << a1.size() << ") size a2: (" << a2.size() << ")" << std::endl;
	{
	    Array<int> numbers(MAX_VAL);
	    int* mirror = new int[MAX_VAL];
	    srand(time(NULL));
	    for (int i = 0; i < MAX_VAL; i++)
	    {
	        const int value = rand();
	        numbers[i] = value;
	        mirror[i] = value;
	    }
	    //SCOPE
	    {
	        Array<int> tmp = numbers;
	        Array<int> test(tmp);
			std::cout << tmp; // prove that  the copy constructor woks
	    }

	    for (int i = 0; i < MAX_VAL; i++)
	    {
	        if (mirror[i] != numbers[i])
	        {
	            std::cerr << "didn't save the same value!!" << std::endl;
	            return 1;
	        }
	    }
	    try
	    {
	        numbers[-2] = 0;
	    }
	    catch(const std::exception& e)
	    {
	        std::cerr << e.what() << '\n';
	    }
	    try
	    {
	        numbers[MAX_VAL] = 0;
	    }
	    catch(const std::exception& e)
	    {
	        std::cerr << e.what() << '\n';
	    }

	    for (int i = 0; i < MAX_VAL; i++)
	    {
	        numbers[i] = rand();
	    }
		std::cout << numbers;
	    delete [] mirror;//
	}
	return (0);
}
