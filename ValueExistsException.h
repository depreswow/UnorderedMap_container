#pragma once
#include <string>
#include <iostream>

class ValueExistsException
	: public std::exception {

	std::string message;

public:

	ValueExistsException();
	const char* what() const override;
};