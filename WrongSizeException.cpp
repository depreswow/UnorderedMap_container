#include "WrongSizeException.h"

WrongSizeException::WrongSizeException(int new_size, int actual_size) {
	message.assign("Wrong size. New size = " + std::to_string(new_size) + ", old size = " + std::to_string(actual_size));
}

const char* WrongSizeException::what() const {
	return message.c_str();
}