//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "iter.hpp"

void sayLen(std::string str)
{
	std::cout << "len: " << str.size() << std::endl;
}

int	main(void)
{
	int array1[5] = {0, 3, 5, 1, 6};
	std::string array2[2] = {"Hello", "World"};

	iter(array1, 5, isEven<int>);
	iter(array2, 2, sayLen);
	(void)array1;
	(void)array2;
	return (0);
}
