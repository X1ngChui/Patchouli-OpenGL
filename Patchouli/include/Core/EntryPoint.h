#pragma once
#include "Application.h"
#include "Log.h"


#ifdef _WIN64
// The entry point of the engine.
int main(int argc, char** argv)
{
	// Initialize the log system.
	Pache::Log::init();
	
	// Create and run the application.
	Pache::Application* app = Pache::createApplication();
	app->run();

	// Release the application.
	delete app;
}

#endif
