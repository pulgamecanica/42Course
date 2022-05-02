//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "ft_containers.hpp"
#ifndef FT
# define FT 0
# define container std
#else
# define container ft
#endif

void say(std::string std) {
	std::cout << std << std::endl;
}

int	main(void) {
	std::cout << "Testing Container -> " << (FT==0 ? "std" : "ft") << std::endl;
	std::cout << WHITE <<"Pulgamecanica greets you :D Welcome to ft_containers" << std::endl;
	std::cout << BLUE << "Vectors" << ENDC << std::endl;
	{
		// container::vector<int> first;                                // empty vector of ints
		// container::vector<int> second (4,100);                       // four ints with value 100
		// container::vector<int> third (second.begin(),second.end());  // iterating through second
		// container::vector<int> fourth (third);                       // a copy of third

		// // the iterator constructor can also be used to construct from arrays:
		// int myints[] = {16,2,77,29};
		// container::vector<int> fifth (myints, myints + sizeof(myints) / sizeof(int) );

		// std::cout << "The contents of fifth are [";
		// for (container::vector<int>::iterator it = fifth.begin(); it != fifth.end(); ++it)
		// 	std::cout << ' ' << *it;
		// std::cout << " ]\n";
	}
	{
		say("Testing Contructors");
		container::vector<int> v1;
		container::vector<int> second (1000,100);

		(void)v1;
	}
	return (0);
}
