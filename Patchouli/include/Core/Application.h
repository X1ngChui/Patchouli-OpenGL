#pragma once
#include "Window.h"
#include "Core/Layer.h"
#include "Core/EventQueue.h"
#include "ImGui/ImGuiLayer.h"

namespace Pache
{
	// Main application class responsible for managing
	// the game loop and layers.
	class Application
	{
	public:
		Application();
		virtual ~Application();

		void run();
		void processEvents() { eventQueue.processEvent(); }

		bool isRunning() { return running; }

		static Application& getApplication() { return *instance; }
		const Window& getWindow() { return *window; }

		// Layer management
		// When a layer is pushed, its ownership is transferred to the applicaiton.
		// When a layer is popped, its ownership is returned to the user.
		void pushLayer(Layer* layer);
		void popLayer(Layer* layer);
		void pushOverlay(Overlay* overlay);
		void popOverlay(Overlay* overlay);

		// Event handler for generic events
		void onEvent(Event& e);
		// Event handler for WindosCloseEvent
		bool onWindowCloseEvent(WindowCloseEvent& e);
	private:
		void enqueueEvent(Event* e);
		void startEventHandlingThread();
	private:
		static Application* instance;

		std::unique_ptr<Window> window;
		ImGuiLayer* imGuiLayer;

		LayerStack layerStack;
		EventQueue eventQueue;

		std::thread eventHandlingThread;

		bool running = true;
	};

	// Factory function to create an instance of the Application class
    // Must be implemented in the client application.
	extern Application* createApplication();
}