#pragma once
#include "Application.h"
#include "Log.h"

#ifdef PLATFORM_WINDOWS

extern Pache::Application* Pache::createApplication();

int main(int argc, char** argv)
{
	Pache::Log::init();

	Pache::Log::coreInfo("Hello, Patchouli!");
	Pache::Log::coreAssert(false, "Hello, Patchouli!");
	
	Pache::Application* app = Pache::createApplication();
	app->run();
	delete app;
}

#endif
