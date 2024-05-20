//If you don't know what those 2 lines are, you should look for forward declaration
class Student;
class Professor;

/*
You're free to edit those class as much as you like, with the only limitation that the classes must perform the required interactions with one another.

You can change base pointer to smart one.
You can change return type to ... Whatever you like.

Anything, as long as you think it's smarter that what is currently writed in this file.
*/

class Course
{
private:
	std::string _name;
	Professor* _responsable;
	std::vector<Student*> _students;
	int _numberOfClassToGraduate;
	int _maximumNumberOfStudent;
public:
	Course(std::string p_name);
	void assign(Professor* p_professor);
	void subscribe(Student* p_student);
};

class Room
{
private:
	long long ID;
	std::vector<Person*> _occupants;

public:
	Room();
	bool canEnter(Person*);
	void enter(Person*);
	void exit(Person*);
	
	void printOccupant();
};

class Classroom : public Room
{
private:
	Course* _currentRoom;

public:
	Classroom();
	void assignCourse(Course* p_course);
};

class SecretarialOffice: public Room
{
private:
	std::vector<Form*> _archivedForms;

public:

};

class HeadmasterOffice : public Room
{
private:

public:

};

class StaffRestRoom : public Room
{
private:

public:

};

class Courtyard : public Room
{
private:

public:

};

class Person
{
private:
	std::string _name;
	Room* _currentRoom;
public:
	Personne(std::string p_name);
	Room* room() {return (_currentRoom);}
};

class Staff : public Person
{
private:

public:
	void sign(Form* p_form);
};

enum class FormType
{
	CourseFinished
	NeedMoreClassRoom
	NeedCourseCreation
	SubscriptionToCourse
};

class Form
{
private:
	FormType _formType;

public:
	Form(FormType p_formType)
	{

	}

	virtual void execute() = 0;
};

class CourseFinishedForm : public Form
{
private:

public:
	void execute();
};

class NeedMoreClassRoomForm : public Form
{
private:

public:
	void execute();
};

class NeedCourseCreationForm : public Form
{
private:

public:
	void execute();
};

class SubscriptionToCourseForm : public Form
{
private:

public:
	void execute();
};

class Student : public Person
{
private:
	std::vector<Course*> _subscribedCourse;

public:
	void attendClass(Classroom* p_classroom);
	void exitClass();
	void graduate(Course* p_course);
};

class Headmaster : public Staff
{
private:
	std::vector<Form*> _formToValidate;
	
public:
	void receiveForm(Form* p_form);
};

class Secretary : public Staff
{
private:

public:
	Form* createForm(FormType p_formType);
	void archiveForm();
};

class Professor : public Staff
{
private:
	Course* _currentCourse;

public:
	void assignCourse(Course* p_course);
	void doClass();
	void closeCourse();
};

enum class Event
{
	RingBell
};

