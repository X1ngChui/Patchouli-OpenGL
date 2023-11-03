#include "Core/Application.h"

namespace Pache
{
	Application::Application()
	{
		window = std::unique_ptr<Window>(Window::create());
		window->setEventCallback(PACHE_BIND_EVENT_CALLBACK(Application::onEvent));
	}

	Application::~Application()
	{
	}

	void Application::onEvent(Event& e)
	{
		Log::coreInfo("{}", e);
	}

	void Application::run()
	{
		while (running)
		{
			window->onUpdate();
		}
	}
}
