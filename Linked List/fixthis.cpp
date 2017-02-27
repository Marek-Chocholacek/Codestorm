// Find all the problems in this code.
// Use valgrind, clang-tidy, clang sanitizers, ...
// Look at all the clang-tidy warnings and try to fix them.

#include <iostream>
#include <string>
#include <vector>
#include <memory>

class Person {
	std::string name;
protected:
	const std::string& getName() const { return name; }
public:
	// Do not worry about the std::move here.
	// We will talk about move semantics later.
	// For now, just assume that it works for strings as for unique_ptr,
	// i.e. after x = std::move(y), y is empty and x contains the
	// original content of y (without copying anything).
	Person(std::string n) : name(std::move(n)) {}
	virtual ~Person() = default;
	virtual void report() const {
		std::cout << "Generic person: " << name << '\n';
	}
};

class Student : public Person {
	std::vector<std::string> courses;
public:
	Student(std::string n) : Person(std::move(n)) {}
	void enroll(std::string course) {
		courses.push_back(std::move(course));
	}
	void report() const override {
		std::cout << "Student " << getName() << ", enrolled in courses:";
		for (auto it : courses) {
			std::cout << "\n\t" << it;
		}
		std::cout << "\n\t(" << courses.size() << " courses in total)\n";
	}
};

class Teacher : public Person {
	std::string office;
	std::string phoneNumber;
public:
	Teacher(std::string n, std::string o, std::string pn)
		: Person(std::move(n)), office(std::move(o)), phoneNumber(std::move(pn)) {}
	void report() const override {
		std::cout << "Teacher " << getName() << ", office "
		          << office << ", phone no. " << phoneNumber << "\n";
	}
};

std::string ask(const std::string& query) {
	std::string response;
	std::cout << query;
	std::getline(std::cin, response);
	return response;
}

void askForCourses(Student& s) {
	std::string course;
	std::cout << "You may now add student's courses, finish with empty line.\n";
	while (true) {
		course = ask("Course: ");
		if (course == "")
			break;
		s.enroll(course);
	}
}

int main() {
	using namespace std;
	vector<unique_ptr<Person> > people;

	string choice;

	while (choice != "X") {
		choice = ask("Add person (T: teacher, S: student, P: generic person, X: end)? ");
		if (choice == "X") { break; }
		if (choice != "T" && choice != "S" && choice != "P") {
			continue;
		}
		string name = ask("Name: ");
		if (choice == "P") {
			people.push_back(make_unique<Person>(name));
		} else if (choice == "S") {
			people.push_back(make_unique<Student>(name));
			askForCourses(dynamic_cast<Student&>(*people.back()));
		} else {
			string office = ask("Office: ");
			string phoneNo = ask("Phone no.: ");
			people.push_back(std::make_unique<Teacher>(name, office, phoneNo));
		}
	}

	cout << "\nPeople:\n";
	for (auto& it : people){
		it->report();
	}
}