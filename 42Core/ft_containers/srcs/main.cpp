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

	{
		say("at() exceptions", YELLOW);
		container::vector<std::string> myvector;
		try {
			std::cout << myvector.at(0) << std::endl;
		} catch (std::out_of_range &e) {
			std::cout << RED << e.what() << ENDC << std::endl;
		}
		try {
			myvector.push_back("Hello World");
			myvector.push_back("Bye World");
			std::cout << myvector.at(9) << std::endl;
		} catch (std::out_of_range &e) {
			std::cout << RED << e.what() << ENDC << std::endl;
		}
	}

	{
		say("back() & front()", YELLOW);
		container::vector<int> myvector;

		myvector.push_back(10);
		while (myvector.back() != 0)
			myvector.push_back ( myvector.back() - 1 );
		std::cout << "myvector contains:";
		for (unsigned i=0; i<myvector.size() ; i++)
			std::cout << ' ' << myvector[i];
		std::cout << '\n';
		myvector.clear();
		myvector.push_back(78);
		myvector.push_back(16);
		myvector.front() -= myvector.back();
		std::cout << "myvector.front() is now " << myvector.front() << '\n';
	}
}

void test_vector_modifiers(void) {
	say("*************", RED);
	say("* MODIFIERS *", RED);
	say("*************", RED);
	{
		say("assign() to a vector which already has elements", YELLOW);
		container::vector<int> myvector;
		say("Before", BLUE);
		myvector.push_back(25);
		while (myvector.back() != 0)
			myvector.push_back ( myvector.back() - 1 );
		print_vector_status(myvector);
		say("After", BLUE);
		myvector.assign(10, 500);
		print_vector_status(myvector);
	}
	{
		say("Testing pop_back() with a vector of 3 elements [100, 200, 300]", YELLOW);
		container::vector<int> myvector;
		int sum (0);
		myvector.push_back (100);
		myvector.push_back (200);
		myvector.push_back (300);
		print_vector_status(myvector);
		
		while (!myvector.empty()) {
			sum+=myvector.back();
			myvector.pop_back();
			say("pop_back()", YELLOW);
			print_vector_status(myvector);
		}
		std::cout << "The elements of myvector add up to " << sum << '\n';
	}
	{
		say("Testing insert()", YELLOW);

		container::vector<int> myvector (3,100);
		container::vector<int>::iterator it;

		say("Original:", YELLOW);
		print_vector_status(myvector);
		it = myvector.begin() + 1;
		it = myvector.insert (it, 200);
		say("Insert single at begin() + 1:", YELLOW);
		print_vector_status(myvector);

		myvector.insert (it, 2, 300);
		say("Insert n(2) elements at iterator begin() + 1 BEFORE last insert:", YELLOW);
		print_vector_status(myvector);
		// "it" no longer valid, get a new one:
		it = myvector.begin();

		say("Another Original vector:", YELLOW);
		container::vector<int> anothervector (2,400);
		print_vector_status(anothervector);
		myvector.insert (it+2 ,anothervector.begin(),anothervector.end());
		say("Insert to the old vector, using iterators begin() & end() of another original vector at begin() + 2", YELLOW);
		print_vector_status(myvector);

		int myarray [] = {501, 502, 503};
		myvector.insert (myvector.begin(), myarray, myarray+3);
		say("Insert to original vector using an array {501, 502, 503} (with iterators) at begin()", YELLOW);
		print_vector_status(myvector);

		std::cout << "myvector contains:";
		for (it=myvector.begin(); it<myvector.end(); it++)
			std::cout << ' ' << *it;
		std::cout << '\n';
	}
	{
		say("Testing erase()", YELLOW);

		container::vector<int> myvector;

		for (int i=1; i<=10; i++) myvector.push_back(i);

		say("erase the 6th element", YELLOW);
		myvector.erase (myvector.begin()+5);
		print_vector_status(myvector);

		say("erase the first 3 elements", YELLOW);
		myvector.erase (myvector.begin(),myvector.begin()+3);
		print_vector_status(myvector);

		std::cout << "myvector contains:";
		for (unsigned i=0; i<myvector.size(); ++i)
			std::cout << ' ' << myvector[i];
		std::cout << '\n';
	}
	{
		say("Testing swap()", YELLOW);

		container::vector<int> foo (3,100);   // three ints with a value of 100
		container::vector<int> bar (5,200);   // five ints with a value of 200		

		say("foo contains:", YELLOW);
		print_vector_status(foo);

		say("bar contains:", YELLOW);
		print_vector_status(bar);

		say("swap()", YELLOW);
		foo.swap(bar);

		say("foo contains:", YELLOW);
		print_vector_status(foo);

		say("bar contains:", YELLOW);
		print_vector_status(bar);
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
			test_vector_modifiers();
		} catch (std::exception &e) {
			std::cout << RED << e.what() << ENDC << std::endl;
		}
	}
	{
		say("************************", RED);
		say("* NON_MEMBER FUNCTIONS *", RED);
		say("************************", RED);
		{
			container::vector<int> foo (3,42);   // three ints with a value of 100
			container::vector<int> bar (2,42);   // two ints with a value of 200

			if (foo==bar) std::cout << "foo and bar are equal\n";
			if (foo!=bar) std::cout << "foo and bar are not equal\n";
			if (foo< bar) std::cout << "foo is less than bar\n";
			if (foo> bar) std::cout << "foo is greater than bar\n";
			if (foo<=bar) std::cout << "foo is less than or equal to bar\n";
			if (foo>=bar) std::cout << "foo is greater than or equal to bar\n";
		}
		say("******************", GREEN);
		{
			container::vector<int> foo (3,200);   // three ints with a value of 100
			container::vector<int> bar (3,200);   // two ints with a value of 200

			if (foo==bar) std::cout << "foo and bar are equal\n";
			if (foo!=bar) std::cout << "foo and bar are not equal\n";
			if (foo< bar) std::cout << "foo is less than bar\n";
			if (foo> bar) std::cout << "foo is greater than bar\n";
			if (foo<=bar) std::cout << "foo is less than or equal to bar\n";
			if (foo>=bar) std::cout << "foo is greater than or equal to bar\n";
		}
		say("******************", GREEN);
		{
			container::vector<int> foo (3,200);   // three ints with a value of 100
			container::vector<int> bar (3,100);   // two ints with a value of 200

			if (foo==bar) std::cout << "foo and bar are equal\n";
			if (foo!=bar) std::cout << "foo and bar are not equal\n";
			if (foo< bar) std::cout << "foo is less than bar\n";
			if (foo> bar) std::cout << "foo is greater than bar\n";
			if (foo<=bar) std::cout << "foo is less than or equal to bar\n";
			if (foo>=bar) std::cout << "foo is greater than or equal to bar\n";
		}
		say("******************", GREEN);
		{
			container::vector<int> foo (10,200);   // three ints with a value of 100
			container::vector<int> bar (103,42);   // two ints with a value of 200

			if (foo==bar) std::cout << "foo and bar are equal\n";
			if (foo!=bar) std::cout << "foo and bar are not equal\n";
			if (foo< bar) std::cout << "foo is less than bar\n";
			if (foo> bar) std::cout << "foo is greater than bar\n";
			if (foo<=bar) std::cout << "foo is less than or equal to bar\n";
			if (foo>=bar) std::cout << "foo is greater than or equal to bar\n";
		}
	}
	return (0);
}
