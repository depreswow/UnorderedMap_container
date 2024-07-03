#include "NoValueException.h"

NoValueException::NoValueException() {
	message.assign("No such value.");
}

const char* NoValueException::what() const {
	return message.c_str();
}