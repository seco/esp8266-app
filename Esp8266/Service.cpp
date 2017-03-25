#include "Service.h"

Service::Service(bool enabled) {
	_enabled = enabled;
}

bool Service::isEnabled() {
	return _enabled;
}

void Service::setEnabled(bool enabled) {
	_enabled = enabled;
}
