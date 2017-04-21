#pragma once

#include <FS.h> // https://github.com/esp8266/Arduino/tree/master/cores/esp8266/FS.h

#include "Service.h"

class FSService : public Service {

	public:

    FSService();

    ~FSService();

    bool isRunning();

    bool start();

    bool stop();

  private:

    bool _running = false;
};
