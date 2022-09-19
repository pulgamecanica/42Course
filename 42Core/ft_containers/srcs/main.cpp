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
	std::cout << "Content: ";
	if (vec.size() < 50) {
		for (container::vector<int>::iterator it = vec.begin(); it != vec.end(); ++it)
			std::cout << ' ' << *it;
		std::cout << std::endl;
	} else {
		unsigned short int i = 0;
		for (container::vector<int>::iterator it = vec.begin(); it != vec.end() && i < 20; ++it, ++i)
			std::cout << ' ' << *it;
		say(" more than 50 elements....", RED);
	}
}

void test_vector_constructors(void) {
	say("****************", RED);
	say("* CONSTRUCTORS *", RED);
	say("****************", RED);

	say("Construct a vector of 100,000 with the value 9", YELLOW);
	container::vector<int> vec1 (100000, 9);
	print_vector_status(vec1);
	say("Construct a vector of 3,000,000 with default value", YELLOW);
	container::vector<int> vec2 (3000000);
	print_vector_status(vec2);
	say("Construct an empty vector", YELLOW);
	container::vector<int> vec3;
	print_vector_status(vec3);
}

void test_vector_iterators(void) {
	;
}

void test_vector_capacity(void) {
	{
		say("******************************************", RED);
		say("* RESIZING A VECTOR & RESERVING CAPACITY *", RED);
		say("******************************************", RED);
	
		container::vector<int> myvector;

		say("Construct a vector [1..9]", YELLOW);
		for (int i=1;i<10;i++) myvector.push_back(i);
		print_vector_status(myvector);
		say("Resize(5)", YELLOW);
		myvector.resize(5);
		print_vector_status(myvector);
		say("Resize(8, 100)", YELLOW);
		myvector.resize(8,100);
		print_vector_status(myvector);
		say("Resize(12)", YELLOW);
		myvector.resize(12);
		print_vector_status(myvector);
		say("Reserve(50)", YELLOW);
		myvector.reserve(50);
		print_vector_status(myvector);
		say("Add 100000 random elements to see how the vector grows in capacity", YELLOW);
		for (int i=1;i<100000;i++) myvector.push_back(rand() % i);
		print_vector_status(myvector);
	}


}

void test_vector_element_access(void) { 
	say("******************", RED);
	say("* ELEMENT ACCESS *", RED);
	say("******************", RED);

	container::vector<std::string> myvector;
	try {
		std::cout << myvector.at(0) << std::endl;
	} catch (std::out_of_range &e) {
		std::cout << RED << e.what() << ENDC << std::endl;
	}
	try {
		std::cout << myvector.at(9) << std::endl;
	} catch (std::out_of_range &e) {
		std::cout << RED << e.what() << ENDC << std::endl;
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
			srand(0);
			test_vector_constructors();
			test_vector_iterators();
			test_vector_capacity();
			test_vector_element_access(); 

		} catch (std::exception &e) {
			std::cout << RED << e.what() << ENDC << std::endl;
		}
	}
	return (0);
}
