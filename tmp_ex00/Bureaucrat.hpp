#pragma once
#include <iostream>

class Bureaucrat {
public:
	/**
	 * OCF
	 */
    Bureaucrat(const std::string& name, const int& grade);
    Bureaucrat(const Bureaucrat& other);
    Bureaucrat& operator=(const Bureaucrat& other);
    ~Bureaucrat();

	/**
	 * Member Function
	 */
	const std::string getName(void) const;
	int getGrade(void) const;

	/**
	 * Exception Class
	 */
	class GradeTooHighException : public std::exception {
		public:
			virtual const char* what() const throw (){
				return "Grade is too high!";
			}
	};
	class GradeTooLowException : public std::exception {
		public:
			virtual const char* what() const throw(){
				return "Grade is too low!";
			}
	};


private:
    Bureaucrat();
	const std::string m_name;
	int m_grade;
    
};

std::ostream &operator<<(std::ostream& out, const Bureaucrat& bureaucrat);

