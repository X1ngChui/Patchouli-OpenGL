#pragma once
#include "Application.h"
#include "Log.h"

#ifdef PACHE_PLATFORM_WINDOWS

int main(int argc, char** argv)
{
	Pache::Log::init();
	
	Pache::Application* app = Pache::createApplication();
	app->run();
	delete app;
}

#endif
