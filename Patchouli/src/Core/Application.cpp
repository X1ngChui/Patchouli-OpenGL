#include "Core/Application.h"

#include "glad/glad.h"

namespace Pache
{
	Application* Application::instance = nullptr;

	Application::Application()
		: eventQueue([this](Event& e) {this->onEvent(e); })
	{
		instance = this;
		window = std::unique_ptr<Window>(Window::create());
		window->setEventCallback([this](Event* e) { this->enqueueEvent(e); });

		imGuiLayer = new ImGuiLayer;
		pushOverlay(imGuiLayer);
	}

	void Application::run()
	{
		// Thread starting
		startEventHandlingThread();

		// Updating loop
		while (running)
		{
			glClearColor(1, 0, 1, 1);
			glClear(GL_COLOR_BUFFER_BIT);

			for (auto layer : layerStack)
			{
				layer->onUpdate();
			}

			imGuiLayer->begin();
			for (auto layer : layerStack)
			{
				layer->onImGuiRender();
			}
			imGuiLayer->end();

			window->onUpdate();
		}
	}

	Application::~Application()
	{
		if (eventHandlingThread.joinable())
		{
			eventHandlingThread.join();
		}
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
		return true;
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

	void Application::startEventHandlingThread()
	{
		eventHandlingThread = std::thread([this]()
			{
				while (this->isRunning())
				{
					this->processEvents();
				}
			});
	}
}
