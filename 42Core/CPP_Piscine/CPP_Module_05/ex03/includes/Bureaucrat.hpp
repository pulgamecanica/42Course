//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __BUREAUCRAT_HPP__
# define __BUREAUCRAT_HPP__

#include <iostream>
#include <exception>
#include "Form.hpp"

class Form;

class Bureaucrat {
	std::string const name;
	int grade;
	void checkGrade(int) throw (std::exception);
	public:
		Bureaucrat();
		Bureaucrat(std::string const, int) throw (std::exception);
		Bureaucrat(const Bureaucrat&);
		~Bureaucrat();
		Bureaucrat& operator= (const Bureaucrat&);
		Bureaucrat& operator++(void) throw (std::exception);
		Bureaucrat operator++(int) throw (std::exception);
		Bureaucrat& operator--(void) throw (std::exception);
		Bureaucrat operator--(int) throw (std::exception);
		std::string const & getName() const;
		void signForm(Form&);
		int getGrade() const;
		void executeForm(Form const & form);
		
		class GradeTooHighException : public std::exception
		{
			public:
				virtual const char* what() const throw();
		} e_gth;
		class GradeTooLowException : public std::exception
		{
			public:
				virtual const char* what() const throw();
		} e_gtl;
};

std::ostream& operator<<(std::ostream&, const Bureaucrat&);

#endif
