#include "Core/Application.h"

namespace Pache
{
	Application::Application()
		: eventQueue([this](Event& e) {this->onEvent(e); })
	{
		window = std::unique_ptr<Window>(Window::create());
		window->setEventCallback([this](Event* e) { this->enqueueEvent(e); });
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

	void Application::pushLayer(Layer* layer)
	{
		layerStack.pushLayer(layer);
	}

	void Application::popLayer(Layer* layer)
	{
		layerStack.popLayer(layer);
	}

	void Application::pushOverlay(Overlay* overlay)
	{
		layerStack.pushOverlay(overlay);
	}

	void Application::popOverlay(Overlay* overlay)
	{
		layerStack.popOverlay(overlay);
	}

	void Application::enqueueEvent(Event* e)
	{
		eventQueue.enqueue(e);
	}
}
