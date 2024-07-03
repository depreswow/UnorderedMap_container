#pragma once
#include <string>
#include <iostream>

class WrongSizeException 
	: public std::exception {
	
	std::string message;

public:

	WrongSizeException(int new_size, int actual_size);
	const char* what() const override;
};