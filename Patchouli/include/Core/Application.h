#pragma once

namespace Pache
{
	class Application
	{
	public:
		Application();
		virtual ~Application();

		void run();
	};

	Application* createApplication();
}