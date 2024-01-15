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

		Renderer::init();

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

			while (!eventQueue.empty())
			{
				auto e = std::unique_ptr<Event>(eventQueue.front());
				eventQueue.pop();
				onEvent(*e);
			}
			
			if (!minimized)
			{
				for (auto layer : layerStack)
				{
					layer->onUpdate(timestep);
				}
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
		dispatcher.dispatch<WindowCloseEvent>([this](WindowCloseEvent& e) -> bool 
			{ 
				return this->onWindowClose(e); 
			}); 
		dispatcher.dispatch<WindowResizeEvent>([this](WindowResizeEvent& e) -> bool
			{
				return this->onWindowResize(e);
			});

		for (auto it = layerStack.end(); it != layerStack.begin();)
		{
			(*(--it))->onEvent(e);
			if (dispatcher.isDealt())
				break;
		}
	}
	
	bool Application::onWindowClose(WindowCloseEvent& e)
	{
		running = false;
		return true;
	}

	bool Application::onWindowResize(WindowResizeEvent& e)
	{
		if (e.getWidth() == 0 || e.getHeight() == 0)
		{
			minimized = true;
			return false;
		}

		minimized = false;
		Renderer::onWindowResize(e.getWidth(), e.getHeight());
		return false;
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
