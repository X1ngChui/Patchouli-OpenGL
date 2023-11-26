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

		vertexArray = std::shared_ptr<VertexArray>(VertexArray::create());

		float vertices[] =
		{
			-0.5f, -0.5f, 0.0f,
			 0.5f,  0.0f, 0.0f,
			-0.5f,  0.5f, 0.0f
		};

		vertexBuffer = std::shared_ptr<VertexBuffer>(VertexBuffer::create(vertices, sizeof(vertices)));

		BufferLayout layout = {
			{ BufferElement::Float3, "position" },
		};

		vertexBuffer->setLayout(layout);
		vertexArray->addVertexBuffer(vertexBuffer);

		unsigned int indices[] = { 0, 1, 2 };
		indexBuffer = std::shared_ptr<IndexBuffer>(IndexBuffer::create(indices, sizeof(indices)));
		vertexArray->setIndexBuffer(indexBuffer);

		std::string vertexSource = R"(
			#version 330 core
			layout(location = 0) in vec3 position;
			
			void main()
			{
				gl_Position = vec4(position, 1.0);
			}
		)";

		std::string fragmentSource = R"(
			#version 330 core
			layout(location = 0) out vec4 color;
			
			void main()
			{
				color = vec4(0.7, 0.2, 0.3, 1.0);
			}
		)";

		shader = std::make_unique<Shader>(vertexSource, fragmentSource);
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
			auto e = std::unique_ptr<Event>(eventQueue.pop());
			onEvent(*e);
		}
	}

	void Application::updateLayers()
	{
		glClearColor(0.2f, 0.2f, 0.2f, 1);
		glClear(GL_COLOR_BUFFER_BIT);

		shader->bind();
		vertexArray->bind();
		glDrawElements(GL_TRIANGLES, indexBuffer->getCount(), GL_UNSIGNED_INT, nullptr);

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
