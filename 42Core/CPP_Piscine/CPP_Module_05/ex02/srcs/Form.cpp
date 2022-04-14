//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "Form.hpp"

void Form::checkGrade(int grade_num) throw(std::exception) {
	if (grade_num <= 0)
		throw e_gth;
	else if (grade_num > 150)
		throw e_gtl;
}

Form::Form(): name(""), min_grade_sign(150), min_grade_execute(150) {
	is_signed = false;
}

Form::Form(std::string const str, int min_gs, int min_ge) throw (std::exception): name(str), min_grade_sign(min_gs), min_grade_execute(min_ge) {
	checkGrade(min_gs);
	checkGrade(min_ge);
	is_signed = false;
}

Form::Form(const Form& param): name(param.name), min_grade_sign(param.min_grade_sign), min_grade_execute(param.min_grade_execute) {is_signed = param.is_signed;}

Form::~Form() {std::cout << "Form " << getName() << " destroyed" << std::endl;}

Form& Form::operator= (const Form& param) {
	is_signed = param.is_signed;
	return (*this);
}

void Form::beSigned(Bureaucrat& b) throw (std::exception) {
	if (b.getGrade() <= min_grade_sign)
		is_signed = true;
	b.signForm(*this);
	if (!is_signed)
		throw (e_ns);
}

std::string const Form::getName() const {return (name);}

int Form::getMinGradeSign() const {return (min_grade_sign);}

int Form::getMinGradeExecute() const {return (min_grade_execute);}

bool Form::getSignedStatus() const {return (is_signed);}

void Form::checkPermissions(Bureaucrat const & executor) const throw (std::exception) {
	if (!is_signed)
		throw (e_ns);
	if (executor.getGrade() > min_grade_execute)
		throw (e_tlts);
}

const char* Form::GradeTooHighException::what() const throw() {return ("[ Exception ] Grade can't be higher than 1...");}

const char* Form::GradeTooLowException::what() const throw() {return ("[ Exception ] Grade can't be lower than 150...");}

const char* Form::NotSignedException::what() const throw() {return ("[ Exception ] Form is not signed...");}

const char* Form::TooLowToSignException::what() const throw() {return ("[ Exception ] Grade too low...");}

std::ostream& operator<<(std::ostream& s, const Form& param) {
	s << "[" << (param.getSignedStatus() == true ? "X" : "O") << "] " << param.getName() << " [sign: " << param.getMinGradeSign() << " | execute: " << param.getMinGradeExecute() << "]";
	return (s);
}
