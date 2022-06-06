#ifndef WARLOCK
# define WARLOCK

#include <iostream>
#include <string>

class Warlock {
	private:
		std::string	_name;
		std::string	_title;
		Warlock(const Warlock &);
		Warlock operator= (const Warlock &);
		Warlock();
	public:
		Warlock(const std::string &, const std::string &);
		~Warlock();
		void setTitle(const std::string &);
		const std::string & getName() const;
		const std::string & getTitle() const;

};

#endif
