#include <iostream>
#include <string>
using namespace std;

void	input_1(void)
{
	string str;

	cout << "Type any sentence:\n";
	cin >> str;
	cout << "Using \033[0;31mcin\033[0m will stop when any separator is founded... this is your string:\n";
	cout << '[' << str << ']' << endl;
}

void	input_(void)
{
	int a;
	string str;

	cout << "Please enter a number: ";
	cin >> a;
	cout << "Now, enter your name:" << endl;
	cin >> str;
	cout << "Hello " << str << ". [" << a << "] is your lucky number." << endl;
}

void	strings_(void)
{
	string str = "An awesome string";
	string other_str("Also a stirng");

	cout << str + "\n";
	cout << other_str << endl;
}

void	new_assign(void)
{
	int a = 1;
	int b(2);

	cout << a + b << endl;
}

int	main(void)
{
	cout << "Hello Dude! :D\n";
	new_assign();
	strings_();
	input_();
	input_1();
	return (0);
}
