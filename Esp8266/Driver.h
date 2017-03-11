#pragma once

#include <Arduino.h>

#include "Configuration.h"

class Driver {

	public:
		// enabled: enables/disables the driver
		// verbose: activates logging output to Serial
		Driver(bool enabled, bool verbose);

		// use this method to know if the driver is enabled or not
		bool isEnabled();

		// enables/disables the driver
		void setEnabled(bool enabled);

		// use this method to know if the logging is verbose
		bool isVerbose();

		// enables/disables logging
		void setVerbose(bool verbose);

	private:
		// enables/disables the driver
		bool _enabled = false;

		// enables/disables serial output for debugging
		bool _verbose = false;
	};
