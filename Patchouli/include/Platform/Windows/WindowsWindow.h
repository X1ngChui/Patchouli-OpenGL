#pragma once
#include "Core/Window.h"
#include "GLFW/glfw3.h"
#include "Renderer/GraphicsContext.h"

namespace Pache
{
	// Concrete implementation of the Window class for the Windows platform
	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowProps& props);
		~WindowsWindow();

		void onUpdate() override;

		virtual unsigned int getWidth() const override { return attrib.width; }
		virtual unsigned int getHeight() const override { return attrib.height; }
		virtual Identifier getTitle() const override { return attrib.title; }

		virtual bool isVSync() const override { return attrib.VSync; }
		virtual void setVSync(bool enabled) override;

		virtual void setEventCallback(const EventCallback& callback) override { attrib.evtCallback = callback; }

		virtual void* getNativeWindow() const override { return window; }

	private:
		GLFWwindow* window;							// GLFW window handle
		GraphicsContext* graphicsContext;

		struct WindowAttrib
		{
			Identifier title;
			unsigned int width, height;
			bool VSync;
			EventCallback evtCallback;
		} attrib;
	};
}