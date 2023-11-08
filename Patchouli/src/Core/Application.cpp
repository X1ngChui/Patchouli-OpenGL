#include "Core/Application.h"

namespace Pache
{
	Application::Application()
	{
		window = std::unique_ptr<Window>(Window::create());
		window->setEventCallback([this](Event& e) { this->onEvent(e); });
	}

	Application::~Application()
	{
	}

	void Application::onEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.dispatch<WindowCloseEvent>([this](WindowCloseEvent& e) -> bool { return this->onWindowCloseEvent(e); });

		for (auto it = layerStack.end(); it != layerStack.begin();)
		{
			(*(--it))->onEvent(e);
			if (dispatcher.isDealt())
				break;
		}
	}
	
	bool Application::onWindowCloseEvent(WindowCloseEvent& e)
	{
		running = false;
		return false;
	}

	void Application::run()
	{
		while (running)
		{
			for (auto layer : layerStack)
			{
				layer->onUpdate();
			}
			window->onUpdate();
		}
	}
}
