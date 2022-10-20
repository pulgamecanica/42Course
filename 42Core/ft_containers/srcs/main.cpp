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
		for (typename container::vector<T>::iterator it = vec.begin(); it != vec.end(); ++it)
			std::cout << ' ' << *it;
		std::cout << std::endl;
	} else {
		unsigned short int i = 0;
		for (typename container::vector<T>::iterator it = vec.begin(); it != vec.end() && i < 20; ++it, ++i)
			std::cout << ' ' << *it;
		say(" more than 50 elements....", RED);
	}
}

template<class T>
void print_vector_status(const container::vector<T> &vec) {
	say("Vector Status", BLUE);
	std::cout << "Size: " << vec.size() << std::endl;
	std::cout << "Max Size: " << vec.max_size() << std::endl;
	std::cout << "Capacity: " << vec.capacity() << std::endl;
	std::cout << "Empty? " << (vec.empty() ? "YES" : "NO") << std::endl;
	std::cout << "Content: ";
	if (vec.size() < 50) {
		for (typename container::vector<T>::const_iterator it = vec.begin(); it != vec.end(); ++it)
			std::cout << ' ' << *it;
		std::cout << std::endl;
	} else {
		unsigned short int i = 0;
		for (typename container::vector<T>::const_iterator it = vec.begin(); it != vec.end() && i < 20; ++it, ++i)
			std::cout << ' ' << *it;
		say(" more than 50 elements....", RED);
	}
}

void test_vector_constructors(void) {
	say("****************", RED);
	say("* CONSTRUCTORS *", RED);
	say("****************", RED);
	{
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
	{
		say("Copy Constructor test", YELLOW);
		container::vector<int> vct(5);
		container::vector<int>::iterator it = vct.begin(), ite = vct.end();

		std::cout << "len: " << (ite - it) << std::endl;
		for (; it != ite; ++it)
			*it = (ite - it);

		it = vct.begin();
		container::vector<int> vct_range(it, --(--ite));
		for (int i = 0; it != ite; ++it)
			*it = ++i * 5;

		it = vct.begin();
		container::vector<int> vct_copy(vct);
		for (int i = 0; it != ite; ++it)
			*it = ++i * 7;
		vct_copy.push_back(42);
		vct_copy.push_back(21);

		std::cout << "\t-- PART ONE --" << std::endl;
		print_vector_status(vct);
		print_vector_status(vct_range);
		print_vector_status(vct_copy);

		vct = vct_copy;
		vct_copy = vct_range;
		vct_range.clear();

		std::cout << "\t-- PART TWO --" << std::endl;
		print_vector_status(vct);
		print_vector_status(vct_range);
		print_vector_status(vct_copy);

	}
}

// class Obj {
// 	public:
// 		Obj(): _n(0) {}
// 		Obj(size_t n): _n(n) {}
// 		size_t m() const {return(_n);}
// 	private:
// 		int _n;
// };
// std::ostream& operator<<(std::ostream& os, const Obj& obj) {
// 	os << obj.m() << std::endl;
//     return os;
// }

void test_vector_iterators(void) {
	say("**************", RED);
	say("* IITERATORS *", RED);
	say("**************", RED);
	{
		const int size = 5;
		container::vector<std::string> vct(size);
		container::vector<std::string>::iterator it(vct.begin());
		container::vector<std::string>::const_iterator ite(vct.end());

		for (int i = 1; ite != it; ++i)
			*it++ = std::string(i, i + 65);
		print_vector_status(vct);

		it = vct.begin();
		ite = vct.begin();

		std::cout << *(++ite) << std::endl;
		std::cout << *(ite++) << std::endl;
		std::cout << *ite++ << std::endl;
		std::cout << *++ite << std::endl;

		it->length();
		ite->length();

		std::cout << *(++it) << std::endl;
		std::cout << *(it++) << std::endl;
		std::cout << *it++ << std::endl;
		std::cout << *++it << std::endl;

		std::cout << *(--ite) << std::endl;
		std::cout << *(ite--) << std::endl;
		std::cout << *--ite << std::endl;
		std::cout << *ite-- << std::endl;

		(*it).length();
		(*ite).length();

		std::cout << *(--it) << std::endl;
		std::cout << *(it--) << std::endl;
		std::cout << *it-- << std::endl;
		std::cout << *--it << std::endl;
	}
	{
		say("Rev iterators", YELLOW);
		const int size = 5;
		container::vector<int> vct(size);
		container::vector<int>::iterator it_ = vct.begin();
		container::vector<int>::reverse_iterator it(it_);

		for (int i = 0; i < size; ++i)
			vct[i] = (i + 1) * 5;
		print_vector_status(vct);

		std::cout << (it_ == it.base()) << std::endl;
		std::cout << (it_ == (it + 3).base()) << std::endl;

		std::cout << *(it.base() + 1) << std::endl;
		// std::cout << *(it - 3) << std::endl;
		std::cout << *(it - 3).base() << std::endl;
		it -= 3;
		std::cout << *it.base() << std::endl;

		std::cout << "TEST OFFSET" << std::endl;
		// std::cout << *(it) << std::endl;
		std::cout << *(it).base() << std::endl;
		std::cout << *(it - 0) << std::endl;
		std::cout << *(it - 0).base() << std::endl;
		std::cout << *(it - 1).base() << std::endl;
	}
	{
		// const int size = 5;
		// container::vector<int> vct(size);
		// container::vector<int>::reverse_iterator it = vct.rbegin();
		// container::vector<int>::const_reverse_iterator ite = vct.rbegin();

		// for (int i = 0; i < size; ++i)
		// 	it[i] = (size - i) * 5;

		// it = it + 5;
		// it = 1 + it;
		// it = it - 4;
		// std::cout << *(it += 2) << std::endl;
		// std::cout << *(it -= 1) << std::endl;

		// *(it -= 2) = 42;
		// *(it += 2) = 21;

		// std::cout << "const_ite +=/-=: " << *(ite += 2) << " | " << *(ite -= 2) << std::endl;

		// std::cout << "(it == const_it): " << (ite == it) << std::endl;
		// std::cout << "(const_ite - it): " << (ite - it) << std::endl;
		// std::cout << "(ite + 3 == it): " << (ite + 3 == it) << std::endl;

		// print_vector_status(vct);
	}
}


static void	checkErase(container::vector<std::string> const &vct, container::vector<std::string>::const_iterator const &it) {
	static int i = 0;
	std::cout << "[" << i++ << "] " << "erase: " << it - vct.begin() << std::endl;
	print_vector_status(vct);
}

void test_vector_capacity(void) {
	say("************", RED);
	say("* CAPACITY *", RED);
	say("************", RED);	
	{
		say("Resizing A VECTOR & RESERVING CAPACITY", YELLOW);
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
	{
		say("erase() tests:", YELLOW);
		container::vector<std::string> vct(10);

		for (unsigned long int i = 0; i < vct.size(); ++i)
			vct[i] = std::string((vct.size() - i), i + 65);
		print_vector_status(vct);

		checkErase(vct, vct.erase(vct.begin() + 2));

		checkErase(vct, vct.erase(vct.begin()));
		checkErase(vct, vct.erase(vct.end() - 1));

		checkErase(vct, vct.erase(vct.begin(), vct.begin() + 3));
		checkErase(vct, vct.erase(vct.end() - 3, vct.end() - 1));

		vct.push_back("Hello");
		vct.push_back("Hi there");
		print_vector_status(vct);
		checkErase(vct, vct.erase(vct.end() - 3, vct.end()));

		vct.push_back("ONE");
		vct.push_back("TWO");
		vct.push_back("THREE");
		vct.push_back("FOUR");
		print_vector_status(vct);
		checkErase(vct, vct.erase(vct.begin(), vct.end()));
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
	{
		say("more back() & front()", YELLOW);
		container::vector<int> vct(7);

		for (unsigned long int i = 0; i < vct.size(); ++i)
		{
			vct.at(i) = (vct.size() - i) * 3;
			std::cout << "vct.at(): " << vct.at(i) << " | ";
			std::cout << "vct[]: " << vct[i] << std::endl;
		}
		print_vector_status(vct);


		container::vector<int> const vct_c(vct);

		print_vector_status(vct_c);

		std::cout << "front(): " << vct.front() << " " << vct_c.front() << std::endl;
		std::cout << "back(): " << vct.back() << " " <<  vct_c.back() << std::endl;


		try {
			vct.at(10) = 42;
		}
		catch (std::out_of_range &e) {
			std::cout << "Catch out_of_range exception!" << std::endl;
		}
		catch (std::exception &e) {
			std::cout << "Catch exception: " << e.what() << std::endl;
		}

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

		say("Insert Test 2", YELLOW);
		container::vector<int> vct(8);
		container::vector<int> vct2;
		container::vector<int> vct3;

		for (unsigned long int i = 0; i < vct.size(); ++i)
			vct[i] = (vct.size() - i) * 3;
		print_vector_status(vct);

		vct2.insert(vct2.end(), 42);
		print_vector_status(vct2);
		vct2.insert(vct2.begin(), 2, 21);
		print_vector_status(vct2);

		vct2.insert(vct2.end() - 2, 42);
		print_vector_status(vct2);

		vct2.insert(vct2.end(), 2, 84);
		print_vector_status(vct2);

		vct2.resize(4);
		print_vector_status(vct2);

		vct2.insert(vct2.begin() + 2, vct.begin(), vct.end());
		vct.clear();
		print_vector_status(vct2);

		print_vector_status(vct);

		for (int i = 0; i < 5; ++i)
			vct3.insert(vct3.end(), i);
		vct3.insert(vct3.begin() + 1, 2, 111);
		print_vector_status(vct3);
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

void test_vector_non_member_functions() {
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
		container::vector<int> foo (3,200);
		container::vector<int> bar (3,100);

		if (foo==bar) std::cout << "foo and bar are equal\n";
		if (foo!=bar) std::cout << "foo and bar are not equal\n";
		if (foo< bar) std::cout << "foo is less than bar\n";
		if (foo> bar) std::cout << "foo is greater than bar\n";
		if (foo<=bar) std::cout << "foo is less than or equal to bar\n";
		if (foo>=bar) std::cout << "foo is greater than or equal to bar\n";
	}
	say("******************", GREEN);
	{
		container::vector<int> foo (10,200);
		container::vector<int> bar (103,42);
		print_vector_status(foo);
		print_vector_status(bar);

		if (foo==bar) std::cout << "foo and bar are equal\n";
		if (foo!=bar) std::cout << "foo and bar are not equal\n";
		if (foo< bar) std::cout << "foo is less than bar\n";
		if (foo> bar) std::cout << "foo is greater than bar\n";
		if (foo<=bar) std::cout << "foo is less than or equal to bar\n";
		if (foo>=bar) std::cout << "foo is greater than or equal to bar\n";
	}
}

// void test_map_constructors() {
// 	say("****************", RED);
// 	say("* CONSTRUCTORS *", RED);
// 	say("****************", RED);
// 	{
// 		say("Construct an empty vector", YELLOW);
// 		std::map<char,int> mymap;
// 		say("Insert Values", YELLOW);
// 		mymap['b'] = 100;
// 		mymap['a'] = 200;
// 		mymap['c'] = 300;
// 		for (std::map<char,int>::iterator it=mymap.begin(); it!=mymap.end(); ++it)
//     		std::cout << it->first << " => " << it->second << std::endl;
// 	}
// }

void bt_test(void) {
	say("****************", RED);
	say("* Testing PAIR *", RED);
	say("****************", RED);
	container::pair<int, char> pair;
	pair = container::make_pair(999, 'a');
	std::cout << pair.first << ", " << pair.second << std::endl;
	say("****************", RED);
	say("* Testing NODE *", RED);
	say("****************", RED);
	ft::Node<ft::pair<int, char> > *root = new ft::Node<ft::pair<int, char> >(ft::make_pair(999, 'a'));
	ft::Node<ft::pair<int, char> > *child1 = new ft::Node<ft::pair<int, char> >(ft::make_pair(1, 'z'));
	ft::Node<ft::pair<int, char> > *child2 = new ft::Node<ft::pair<int, char> >(ft::make_pair(4999, 'y'));
	root->put_left(child1);
	root->put_right(child2);
	root->display(root);

	// ft::RedBlackTree<ft::pair<int, char> > rbt;
	// ft::pair<int, char> p1 = ft::make_pair(9, 'a');
	// ft::pair<int, char> p2 = ft::make_pair(2, 'b');
	// ft::pair<int, char> p3 = ft::make_pair(3, 'c');
	// ft::pair<int, char> p4 = ft::make_pair(4, 'd');
	// ft::pair<int, char> p5 = ft::make_pair(5, 'e');
	// rbt.insert(p1);
	// rbt.insert(p2);
	// rbt.insert(p2);
	// rbt.insert(p3);
	// rbt.insert(p4);
	// rbt.insert(p5);
	// rbt.displayRBT();
	// (void)rbt;
	delete(root);
	delete(child1);
	delete(child2);
}

int	main(void) {
	// std::cout << "Testing Container -> " << (FT == 0 ? "std" : "ft") << std::endl;
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
			test_vector_non_member_functions();
		} catch (std::exception &e) {
			std::cout << RED << e.what() << ENDC << std::endl;
		}
	}
	{
		// test_map_constructors();
		bt_test();
	}
	
	return (0);
}
