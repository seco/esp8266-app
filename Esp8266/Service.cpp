#include "Service.h"

Service::Service(bool enabled, bool verbose) {
	_enabled = enabled;
	_verbose = verbose;
}

bool Service::isEnabled() {
	return _enabled;
}

void Service::setEnabled(bool enabled) {
	_enabled = enabled;
}

bool Service::isVerbose() {
	return _verbose;
}

void Service::setVerbose(bool verbose) {
	_verbose = verbose;
}
