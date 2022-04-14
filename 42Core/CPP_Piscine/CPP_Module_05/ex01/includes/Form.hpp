//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __FORM_HPP__
# define __FORM_HPP__

#include <iostream>
#include "Bureaucrat.hpp"

class Bureaucrat;

class Form {
	std::string const name;
	bool is_signed;
	int const min_grade_sign;
	int const min_grade_execute;
	void checkGrade(int) throw (std::exception);
	public:
		Form();
		Form(const Form&);
		Form(std::string const, int, int) throw (std::exception);
		~Form();
		Form& operator= (const Form&);
		std::string const getName() const;
		int getMinGradeSign() const;
		int getMinGradeExecute() const;
		bool getSignedStatus() const;
		void beSigned(Bureaucrat&) throw (std::exception);
		
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

std::ostream&	operator<<(std::ostream&, const Form&);

#endif

