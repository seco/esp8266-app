#pragma once

#include "Configuration.h"

class Service {

	public:
		// enabled: enables/disables the service
		// verbose: activates logging output to Serial
		Service(bool enabled, bool verbose);

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
