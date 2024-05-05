//***************************//
//*Template by pulgamecanica*//
//***************************//

#include <algorithm>

#include "School.inc"

#include "Course.hpp"
#include "Student.hpp"
#include "Professor.hpp"
#include "CourseList.hpp"

Course::Course(const std::string & name, int number_of_class_to_graduate, int course_capacity): name_(name), responsable_(nullptr),
	number_of_class_to_graduate_(number_of_class_to_graduate), course_capacity_(course_capacity) {
	;
	if (DEBUG)
		std::cout << YELLOW << "[Course] " << GREEN << "CREATE\t" << ENDC << "Course " << BLUE << name << ENDC << std::endl;
	CourseList::GetInstance()->add(this);

}

Course::~Course() {
	responsable_ = nullptr;
	students_.clear();
	CourseList::GetInstance()->remove(this);
	if (DEBUG)
    	std::cout << "Course " << *this << " destroyed" << std::endl;
}

bool Course::hasStudent(Student * s) const {
	return std::find(students_.begin(), students_.end(), s) != students_.end();
}

const Professor * Course::getResponsable() const {
	return responsable_;
}

void Course::assign(Professor* p_professor) {
	responsable_ = p_professor;
	if (DEBUG) {
		if (p_professor)
			std::cout << YELLOW << "[Course] " << GREEN << "ASSIGN\t" << ENDC << *p_professor << " to " << BLUE << name_ << ENDC << std::endl;
		else
			std::cout << YELLOW << "[Course] " << GREEN << "UNASSIGN\t" << ENDC << " to " << BLUE << name_ << ENDC << std::endl;
	}
}

void Course::subscribe(Student* p_student) {
	if(!hasStudent(p_student) && students_.size() < (long unsigned)course_capacity_) {
		students_.push_back(p_student);
		if (DEBUG)
			std::cout << YELLOW << "[Course] " << GREEN << "SUBSCRIBE\t" << ENDC << *p_student << " to " << *this << std::endl;
		return ;
	}
	if (DEBUG) {
		if (hasStudent(p_student))
			std::cout << YELLOW << "[Course] " << RED << "SUBSCRIBE FAIL\t" << ENDC << *p_student << " is already subscribed to " << *this << std::endl;
		else if (students_.size() >= (long unsigned)course_capacity_)
			std::cout << YELLOW << "[Course] " << RED << "SUBSCRIBE FAIL\t" << ENDC << *this << " is full" << std::endl;
	}
}

void Course::finish() {
	std::cout << BLUE << "Graduation ceremony!" << ENDC << std::endl;
	for (std::vector<Student *>::const_iterator i = students_.begin(); i != students_.end(); ++i) {
		if ((*i)->getCourseAttendance(this) >= number_of_class_to_graduate_)
			std::cout << " > " << **i << GREEN << " will graduate upon this cursus ending season!" << ENDC << std::endl;
		else
			std::cout << " > " << **i << RED << " cannot graduate just yet! :(" << YELLOW << " total assistance: " << ENDC << (*i)->getCourseAttendance(this) << std::endl;
	}
}

std::ostream& Course::loadCourseInfo(std::ostream& s) const {
	s << name_ << "|" << number_of_class_to_graduate_ << " [" << students_.size() << "/" << course_capacity_ << "]";
	if (responsable_)
		s << " Responable: " << *responsable_;
	else
		s << RED << " No Responable" << ENDC;
	return s;
}

std::ostream& operator<<(std::ostream& s, const Course& c) {
	return c.loadCourseInfo(s);
}
