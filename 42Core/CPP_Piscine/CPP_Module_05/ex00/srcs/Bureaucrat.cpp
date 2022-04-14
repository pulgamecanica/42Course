//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "Bureaucrat.hpp"

Bureaucrat::Bureaucrat(): name("") {
	grade = 150;
}

void Bureaucrat::checkGrade(int grade_num) throw(std::exception) {
	if (grade_num <= 0)
		throw e_gth;
	else if (grade_num > 150)
		throw e_gtl;
}

Bureaucrat::Bureaucrat(std::string const str, int num) throw (std::exception): name(str) {
	checkGrade(num);
	grade = num;
}

Bureaucrat::Bureaucrat(const Bureaucrat& param): name(param.name), grade(param.grade) {}

Bureaucrat::~Bureaucrat() {std::cout << "Bureaucrat " <<  name << "(" << grade << ")" <<" destroyed" << std::endl;}

Bureaucrat& Bureaucrat::operator= (const Bureaucrat& param) {
	grade = param.grade;
	return (*this);
}

Bureaucrat& Bureaucrat::operator++(void) throw (std::exception) {
	checkGrade(grade - 1);
	grade--;
	return (*this);
}

Bureaucrat Bureaucrat::operator++(int) throw (std::exception) {
	checkGrade(grade - 1);
	Bureaucrat tmp(*this);
	grade--;
	return (tmp);

}

Bureaucrat& Bureaucrat::operator--(void) throw (std::exception) {
	checkGrade(grade + 1);
	grade++;
	return (*this);
}

Bureaucrat Bureaucrat::operator--(int) throw (std::exception) {
	checkGrade(grade + 1);
	Bureaucrat tmp(*this);
	grade++;
	return (tmp);
}

std::string const & Bureaucrat::getName() const {return (name);}

int Bureaucrat::getGrade() const {return (grade);}

const char* Bureaucrat::GradeTooHighException::what() const throw() {return ("[ Exception ] Grade can't be higher than 1...");}

const char* Bureaucrat::GradeTooLowException::what() const throw() {return ("[ Exception ] Grade can't be lower than 150...");}

std::ostream& operator<<(std::ostream& s, const Bureaucrat& param) {
	s << param.getName() << ", bureaucrat grade " <<  param.getGrade() << ".";
	return (s);
}
