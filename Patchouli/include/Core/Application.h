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

		template <IsLayer T, typename...Args>
		T* createLayer(Args&&...args)
		{
			return layerStack.createLayer<T>(std::forward<Args>(args)...);
		}

		void onEvent(Event& e);
		bool onWindowCloseEvent(WindowCloseEvent& e);
	private:
		std::unique_ptr<Window> window;
		LayerStack layerStack;
		bool running = true;
	};

	extern Application* createApplication();
}