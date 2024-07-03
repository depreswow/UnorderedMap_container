#pragma once
#include <string>
#include <iostream>

class BadLoadFactor
	: public std::exception {

	std::string message;

public:

	BadLoadFactor(double in_factor);
	const char* what() const override;
};