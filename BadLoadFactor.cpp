#include "BadLoadFactor.h"

BadLoadFactor::BadLoadFactor(double in_factor) {
	message.assign("Bad LoadFactor. LoadFactor = " + std::to_string(in_factor) + ". LoadFactor should be between 0 and 1.");
}

const char* BadLoadFactor::what() const {
	return message.c_str();
}