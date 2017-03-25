#pragma once

#include <Arduino.h>
#include <ArduinoLog.h> // https://github.com/thijse/Arduino-Log

#include "Configuration.h"

class Service {

	public:
		// enabled: enables/disables this service
		Service(bool enabled);

		// use this method to know if this driver is enabled or not
		bool isEnabled();

		// enables/disables this driver
		void setEnabled(bool enabled);

	private:
		// enables/disables this service
		bool _enabled = false;
	};
