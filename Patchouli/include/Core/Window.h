#pragma once

#include "Events/Event.h"
#include "Events/ApplicationEvent.h"
#include "Events/KeyboardEvent.h"
#include "Events/MouseEvent.h"
#include "Events/WindowEvent.h"

namespace Pache
{
	// Structure defining properties for a window,
	// including width, height, and title.
	struct WindowProps
	{
		unsigned int width;
		unsigned int height;
		std::string title;

		WindowProps(unsigned int width = 1280,
			unsigned int height = 720,
			const std::string title = "Patchouli Engine")
			:width(width), height(height), title(title)
		{
		}
	};


	class Window
	{
	public:
		using EventCallback = std::function<void(Event*)>;

		virtual ~Window() {}

		virtual void onUpdate() = 0;

		virtual unsigned int getWidth() const = 0;
		virtual unsigned int getHeight() const = 0;
		virtual Identifier getTitle() const = 0;

		virtual void setEventCallback(const EventCallback& callback) = 0;
		virtual void setVSync(bool enabled) = 0;
		virtual bool isVSync() const = 0;

		virtual void* getNativeWindow() const = 0;

		// Static factory method to create a window with optional properties
		static Window* create(const WindowProps& props = WindowProps());
	};
}