#pragma once

#include "Event.h"

namespace Pache
{
	class WindowCloseEvent : public Event {
	public:
		WindowCloseEvent() {}

		virtual constexpr int getCategoryFlag() const override
		{
			return EventCategory::EventCategoryWindow;
		}

		static constexpr EventType getStaticType() { return EventType::WindowClosed; }
		virtual constexpr EventType getEventType() const override { return EventType::WindowClosed; }

		std::string toString() const override 
		{
			return "WindowCloseEvent";
		}
	};


	class WindowResizeEvent : public Event
	{
	public:
		WindowResizeEvent(unsigned int width, unsigned int height)
			: m_windowWidth(width), m_windowHeight(height) {}
		
		unsigned int getWidth() const { return m_windowWidth; }
		unsigned int getHeight() const { return m_windowHeight; }

		virtual constexpr int getCategoryFlag() const override
		{
			return EventCategory::EventCategoryWindow;
		}

		static constexpr EventType getStaticType() { return EventType::WindowResize; }
		virtual constexpr EventType getEventType() const override { return EventType::WindowResize; }

		std::string toString() const override 
		{
			return fmt::format("WindowResizeEvent (Size: ({}, {}))", m_windowWidth, m_windowHeight);
		}

	private:
		unsigned int m_windowWidth, m_windowHeight;
	};
}

