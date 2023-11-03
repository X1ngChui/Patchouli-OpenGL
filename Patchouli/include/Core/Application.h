#pragma once
#include "Window.h"

namespace Pache
{
	class Application
	{
	public:
		Application();
		virtual ~Application();

		void run();
		void onEvent(Event& e);
	private:
		std::unique_ptr<Window> window;
		bool running = true;
	};

	Application* createApplication();
}