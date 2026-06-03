#include "Bureaucrat.hpp"
#include "iostream"

Bureaucrat::Bureaucrat(const std::string& name, const int& grade) : m_name(name), m_grade(grade) {
	if (grade < 1)
	{
		std::cout << "GradeTooHighException called" << std::endl;
		throw Bureaucrat::GradeTooHighException();
	}
	else if (grade > 150)
	{
		std::cout << "GradeTooLowException called" << std::endl;
		throw Bureaucrat::GradeTooLowException();
	}
}

Bureaucrat::Bureaucrat(const Bureaucrat& other) : m_name(other.m_name), m_grade(other.m_grade) {}

Bureaucrat& Bureaucrat::operator=(const Bureaucrat& other) {
    if (this != &other) {
		this->m_grade = other.m_grade;
    }
    return *this;
}

Bureaucrat::~Bureaucrat() {}

std::ostream &operator<<(std::ostream& out, const Bureaucrat& bureaucrat) {
	out << bureaucrat.getName() << ", bureaucrat grade " << bureaucrat.getGrade() << ".";
	return out;
}

const std::string Bureaucrat::getName(void) const
{
	return m_name;
}

int Bureaucrat::getGrade(void) const
{
	return m_grade;
}
