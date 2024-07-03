#include "ValueExistsException.h"

ValueExistsException::ValueExistsException() {
	message.assign("Value already exists.");
}

const char* ValueExistsException::what() const {
	return message.c_str();
}