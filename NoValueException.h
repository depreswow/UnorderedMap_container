#pragma once
#include <string>
#include <iostream>

class NoValueException
	: public std::exception {

	std::string message;

public:

	NoValueException();
	const char* what() const override;
};