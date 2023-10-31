#pragma once
#include "Application.h"

#ifdef PLATFORM_WINDOWS

extern Pache::Application* Pache::createApplication();

int main(int argc, char** argv)
{
	Pache::Application* app = Pache::createApplication();
	app->run();
	delete app;
}

#endif
