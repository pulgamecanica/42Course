#include <vector>

//If you don't know what those 2 lines are, you should look for forward declaration
class Student;
class Professor;
class Form;

/*
You're free to edit those class as much as you like, with the only limitation that the classes must perform the required interactions with one another.

You can change base pointer to smart one.
You can change return type to ... Whatever you like.

Anything, as long as you think it's smarter that what is currently writed in this file.
*/

class Course {
private:
	std::string _name;
	Professor* _responsable;
	std::vector<Student*> _students;
	int _numberOfClassToGraduate;
	int _maximumNumberOfStudent;
	
public:
	Course(std::string p_name): _name(p_name) {
		_numberOfClassToGraduate = 210;
		_maximumNumberOfStudent = 20;
	};

	bool hasStudent(Student * s) {
		return std::find(_students.begin(), _students.end(), s) != _students.end();
	}

	void assign(Professor* p_professor); {
		_responsable = p_professor;
	}

	void subscribe(Student* p_student) {
		if(!hasStudent(p_student) && _students.size() < _maximumNumberOfStudent)
			_students.push_back(p_student);
	}
};

class Room {
private:
	static _gID = 0;
	long long ID;
	std::vector<Person*> _occupants;
public:
	Room() {
		ID = _gID++;
	};

	virtual bool canEnter(Person* p_person) {
		return std::find(_occupants.begin(), _occupants.end(), p_person) != _occupants.end();
	};

	bool enter(Person* p_person) {
		if (canEnter(p_person)) {
			_occupants.push_back(p_person)
			return true
		}
		return false
	};

	void exit(Person*) {
		std::vector<Person*>::iterator it = std::find(_occupants.begin(), _occupants.end(), p_person);
		if (it != _occupants.end())
			_occupants.erase(it);
	}
	
	void printOccupant() {
		for (std::vector<Person *>::iterator i = _occupants.begin(); i != _occupants.end(); ++i) {
			std::cout << "Person: " << i << std::endl;
		}
	}
};

class Classroom : public Room {
private:
	Course* _currentRoom;

public:
	Classroom();

	void assignCourse(Course* p_course) {
		_currentRoom = p_course;
	}
};

class SecretarialOffice: public Room {
private:
	std::vector<Form*> _archivedForms;
	bool hasForm(Form * p_form) {
		return std::find(_archivedForms.begin(), _archivedForms.end(),  p_form) != _archivedForms.end()
	}
public:
	void addForm(Form * p_form) {
		if (!hasForm(p_form))
			_archivedForms.push_back(p_form)
	}

};

class HeadmasterOffice : public Room {
private:

public:

};

class StaffRestRoom : public Room {
private:

public:

};

class Courtyard : public Room {
private:

public:

};

class Person {
private:
	std::string _name;
	Room* _currentRoom;
public:
	Person(std::string p_name): _name(p_name) {
		;
	}
	
	void setRoom(Room *p_room) {
		_currentRoom = p_room;
	}
	
	Room* room() {
		return (_currentRoom);
	}
};

class Staff : public Person {
private:

public:
	void sign(Form* p_form) {
		std::cout << p_form << "signed" << std::endl;
	}
};

enum class FormType {
	CourseFinished,
	NeedMoreClassRoom,
	NeedCourseCreation,
	SubscriptionToCourse
};

class Form {
private:
	FormType _formType;
	bool _signed_by_staff;
public:
	Form(FormType p_formType): _formType(p_formType) {
		bool _signed_by_staff = false;
	}

	virtual void execute() = 0;
};

class CourseFinishedForm : public Form
{
private:

public:
	CourseFinishedForm(): Form(CourseFinished) {
		;
	}

	void execute() {
		if (_signed_by_staff)
			std::cout << "Execute Form 66!" << std::endl;
		else
			std::cout << "Can't execute!" << std::endl;
	}
};

class NeedMoreClassRoomForm : public Form
{
private:

public:
	NeedMoreClassRoomForm(): Form(NeedMoreClassRoom) {
		;
	}

	void execute() {
		if (_signed_by_staff)
			std::cout << "Execute Classroom Request!" << std::endl;
		else
			std::cout << "Can't execute!" << std::endl;
	}
};

class NeedCourseCreationForm : public Form
{
private:

public:
	void execute() {
		if (_signed_by_staff)
			std::cout << "Execute Course Creation Request!" << std::endl;
		else
			std::cout << "Can't execute!" << std::endl;
	}
};

class SubscriptionToCourseForm : public Form
{
private:

public:
	void execute() {
		if (_signed_by_staff)
			std::cout << "Execute Subscription to Course Request!" << std::endl;
		else
			std::cout << "Can't execute!" << std::endl;
	}
};

class Student : public Person
{
private:
	std::vector<Course*> _subscribedCourse;
	Classroom * _currentClassRoom;
public:
	void attendClass(Classroom* p_classroom) {
		bool enter_res = p_classroom.enter();
		if (enter_res)
			_currentClassRoom = p_classroom;
	}

	void exitClass() {
		_currentClassRoom.exit()
		_currentClassRoom = NULL;
	}

	void graduate(Course* p_course) {
		if (p_course->hasStudent(this))
			std::cout << "Graduating from " << p_course << std::endl;
	}
};

class Headmaster : public Staff
{
private:
	std::vector<Form*> _formToValidate;
	
public:
	void receiveForm(Form* p_form) {
		_formToValidate = p_form;
		// p_form->execute();
	}
};

class Secretary : public Staff {
private:

public:
	Form* createForm(FormType p_formType) {
		switch (p_formType) {
		case CourseFinished : return new CourseFinishedForm();
		case NeedMoreClassRoom : return new NeedCourseCreationForm();
		case NeedCourseCreation : return new NeedCourseCreationForm();
		case SubscriptionToCourse : return new SubscriptionToCourse();
		}
	}

	void archiveForm() {
		std::cout << "Achieve the Form" << std::endl;
	}
};

class Professor : public Staff
{
private:
	Course* _currentCourse;
public:
	void assignCourse(Course* p_course): _currentCourse(p_course) {
		;
	}

	void doClass() {
		std::cout << "Teaching now" << std::endl;
	}

	void closeCourse() {
		std::cout << "Closing course" << std::endl;
	}
};

enum class Event {
	RingBell
};

