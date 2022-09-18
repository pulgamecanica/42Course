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

void say(std::string std, std::string color = ENDC) {
	std::cout << color << std << ENDC << std::endl;
}

template<class T>
void print_vector_status(container::vector<T> &vec) {
	say("Vector Status", BLUE);
	std::cout << "Size: " << vec.size() << std::endl;
	std::cout << "Max Size: " << vec.max_size() << std::endl;
	std::cout << "Capacity: " << vec.capacity() << std::endl;
	std::cout << "Empty? " << (vec.empty() ? "YES" : "NO") << std::endl;
	if (vec.size() < 50) {
		std::cout << "Content: ";
		for (container::vector<int>::iterator it = vec.begin(); it != vec.end(); ++it)
				std::cout << ' ' << *it;
		std::cout << std::endl;
	}
	say("End of Info", BLUE);
}

void test_vector_constructors(void) {
	say("Construct an array of 100 elements from 0 to 1", RED);
	container::vector<int> vec1;
	vec1.reserve(999);
	print_vector_status(vec1);
	say("Construct an array of 100,000 elements with an iterator", RED);
	container::vector<int> vec2 (100000, 9);
	print_vector_status(vec2);
}

void test_vector_iterators(void) {
	;
}

void test_vector_capacity(void) {
	{
		say("*Resize*", BLUE);
		container::vector<int> myvector;

		for (int i=1;i<10;i++) myvector.push_back(i);
		// set some initial content:
		print_vector_status(myvector);
		myvector.resize(5);
		print_vector_status(myvector);
		myvector.resize(8,100);
		print_vector_status(myvector);
		myvector.resize(12);
		print_vector_status(myvector);


		std::cout << "myvector contains:";
		// for (size_t i=0;i<myvector.size();i++)
		for (container::vector<int>::iterator it = myvector.begin(); it != myvector.end(); ++it)
			std::cout << ' ' << *it;
		std::cout << '\n';
	}


}

int	main(void) {
	std::cout << "Testing Container -> " << (FT == 0 ? "std" : "ft") << std::endl;
	say("Pulgamecanica greets you :D Welcome to ft_containers", WHITE);
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
		try {
			
			test_vector_constructors();
			test_vector_iterators();
			test_vector_capacity();

		} catch (std::exception &e) {
			std::cout << RED << e.what() << std::endl;
		}
	}
	return (0);
}
