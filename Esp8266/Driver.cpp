#include "Driver.h"

Driver::Driver(bool enabled, bool verbose) {
	_enabled = enabled;
	_verbose = verbose;
}

bool Driver::isEnabled() {
	return _enabled;
}

void Driver::setEnabled(bool enabled) {
	_enabled = enabled;
}

bool Driver::isVerbose() {
	return _verbose;
}

void Driver::setVerbose(bool verbose) {
	_verbose = verbose;
}
