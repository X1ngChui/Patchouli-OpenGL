#pragma once
#include "Window.h"
#include "Core/Layer.h"

namespace Pache
{
	class Application
	{
	public:
		Application();
		virtual ~Application();

		void run();

		void pushLayer(Layer* layer);
		void popLayer(Layer* layer);
		void pushOverlay(Overlay* overlay);
		void popOverlay(Overlay* overlay);

		void onEvent(Event& e);
		bool onWindowCloseEvent(WindowCloseEvent& e);
	private:
		std::unique_ptr<Window> window;
		LayerStack layerStack;
		bool running = true;
	};

	extern Application* createApplication();
}