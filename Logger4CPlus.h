#pragma once

#include <log4cplus/logger.h>
#include <log4cplus/fileappender.h>
#include <log4cplus/layout.h>
#include <log4cplus/ndc.h>
#include <log4cplus/helpers/loglog.h>
#include <log4cplus/helpers/property.h>
#include <log4cplus/loggingmacros.h>
#include <iostream>
#include <string>

using namespace log4cplus;
using namespace std;


class Logger4CPlus
{
	
public:
	Logger4CPlus();
	~Logger4CPlus();
	static void Instantiate();
	static bool Instance();
	
};

