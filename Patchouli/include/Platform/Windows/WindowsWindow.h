#pragma once
#include "Core/Window.h"
#include "GLFW/glfw3.h"

namespace Pache
{
	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowProps& props);
		~WindowsWindow();

		void onUpdate() override;

		unsigned int getWidth() const override { return attrib.width; }
		unsigned int getHeight() const override { return attrib.height; }
		const std::string& getTitle() const override { return attrib.title; }

		bool isVSync() const override { return attrib.VSync; }
		void setVSync(bool enabled) override;

		void setEventCallback(const EventCallback& callback) override { attrib.evtCallback = callback; }

	private:
		GLFWwindow* window;

		struct WindowAttrib
		{
			std::string title;
			unsigned int width, height;
			bool VSync;
			EventCallback evtCallback;
		} attrib;
	};
}