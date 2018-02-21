#include "Logger4CPlus.h"

static Logger4CPlus *logger4CPlus;
Logger4CPlus::Logger4CPlus()
{
}


Logger4CPlus::~Logger4CPlus()
{
	if (logger4CPlus) {
		delete logger4CPlus;
	}
	logger4CPlus = NULL;
	log4cplus::Logger::shutdown();
}

void Logger4CPlus::Instantiate()
{
	if (logger4CPlus == NULL) {
		logger4CPlus = new Logger4CPlus();
		system("cls");
		log4cplus::initialize();
		helpers::LogLog::getLogLog()->setInternalDebugging(true);
		SharedFileAppenderPtr append_1(
			new RollingFileAppender(LOG4CPLUS_TEXT("Log/Hangman Server.log"), 5 * 1024, 5,
			false, true));
		append_1->setName(LOG4CPLUS_TEXT("First"));
		append_1->setLayout(std::auto_ptr<Layout>(new TTCCLayout()));
		append_1->getloc();
		Logger::getRoot().addAppender(SharedAppenderPtr(append_1.get()));
		Logger root = Logger::getRoot();
		system("cls");
	}
}

bool Logger4CPlus::Instance()
{
	if (logger4CPlus == NULL) {
		return false;
	}
	return true;
}