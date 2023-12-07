#include "Core/Application.h"

#include "GLFW/glfw3.h"

namespace Pache
{
	Application* Application::instance = nullptr;

	Application::Application()
	{
		instance = this;

		window = std::unique_ptr<Window>(Window::create());
		window->setEventCallback([this](Event* e) { eventQueue.push(e); });

		imGuiLayer = new ImGuiLayer;
		pushOverlay(imGuiLayer);
	}

	Application::~Application()
	{
	}

	void Application::run()
	{
		while (running)
		{
			float time = (float)glfwGetTime();
			Timestep timestep = time - lastFrameTime;
			lastFrameTime = time;

			processEvents();
			
			for (auto layer : layerStack)
			{
				layer->onUpdate(timestep);
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

	void Application::processEvents()
	{
		while (!eventQueue.empty())
		{
			auto e = std::unique_ptr<Event>(eventQueue.pop());
			onEvent(*e);
		}
	}

	void Application::onEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.dispatch<WindowCloseEvent>([this](WindowCloseEvent& e) -> bool 
			{ 
				return this->onWindowCloseEvent(e); 
			});

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
