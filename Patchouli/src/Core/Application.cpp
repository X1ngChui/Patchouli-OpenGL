#include "Core/Application.h"

#include "glad/glad.h"

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

		glGenVertexArrays(1, &vertexArray);
		glBindVertexArray(vertexArray);

		glGenBuffers(1, &vertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

		float vertices[] =
		{
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			-0.5f,  0.5f, 0.0f
		};

		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

		glGenBuffers(1, &indexBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);

		unsigned int indices[] = { 0, 1, 2 };
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	}

	Application::~Application()
	{
	}

	void Application::run()
	{
		while (running)
		{
			processEvents();
			updateLayers();
		}
	}

	void Application::processEvents()
	{
		while (!eventQueue.empty())
		{
			Event* e = eventQueue.pop();
			onEvent(*e);
			delete e;
		}
	}

	void Application::updateLayers()
	{
		glClearColor(1, 0, 1, 1);
		glClear(GL_COLOR_BUFFER_BIT);

		glBindVertexArray(vertexArray);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);

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

	void Application::onEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.dispatch<WindowCloseEvent>([this](WindowCloseEvent& e) -> bool 
			{ 
				return this->onWindowCloseEvent(e); 
			});

		// Pache::Log::coreInfo(e);

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
