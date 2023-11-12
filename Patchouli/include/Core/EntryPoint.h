#pragma once
#include "Application.h"
#include "Log.h"

#ifdef PACHE_PLATFORM_WINDOWS

// The entry point of the engine.
int main(int argc, char** argv)
{
	// Initialize the log system.
	Pache::Log::init();
	
	// Create and run the application.
	Pache::Application* app = Pache::createApplication();
	std::thread eventProcessingThread([&app]()
		{
			while (app->isRunning())
			{
				app->processEvents();
			}
		});
	app->run();

	// Waiting for all threads to terminate.
	eventProcessingThread.join();

	// Release the application.
	delete app;
}

#endif
