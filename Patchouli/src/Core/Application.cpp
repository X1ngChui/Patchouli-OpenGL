#include "Core/Application.h"

#include "glad/glad.h"

namespace Pache
{
	Application* Application::instance = nullptr;

	Application::Application()
	{
		instance = this;
		window = std::unique_ptr<Window>(Window::create());
		window->setEventCallback([this](Event* e) { (this->eventQueue).push(e); });

		imGuiLayer = new ImGuiLayer;
		pushOverlay(imGuiLayer);
	}

	Application::~Application()
	{
	}

	void Application::run()
	{
		// Application running loop
		while (running)
		{
			glClearColor(1, 0, 1, 1);
			glClear(GL_COLOR_BUFFER_BIT);

			while (!eventQueue.empty())
			{
				Event* e = eventQueue.pop();
				onEvent(*e);

				// TODO:
				// The current implementation of Event utilizes raw new and raw delete for memory management.
				// In the future, the plan is to transition to a obeject pool approach for enhanced efficiency and  resource utilization.
				// e->release();
				delete e;
			}

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
}
