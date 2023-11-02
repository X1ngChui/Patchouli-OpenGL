#pragma once

#include "Event.h"

namespace Pache
{
	class MouseButtonEvent : public Event
	{
	public:
		inline int getMouseButton() const { return m_button; }

		virtual constexpr int getCategoryFlag() const override
		{
			return EventCategory::EventCategoryInput | EventCategory::EventCategoryMouse | EventCategory::EventCategoryMouseButton;
		}
	protected:
		int m_button;

		MouseButtonEvent(int button)
			: m_button(button)
		{
		}
	};


	class MouseButtonPressedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonPressedEvent(int button)
			: MouseButtonEvent(button)
		{
		}

		static constexpr EventType getStaticType() { return EventType::MouseButtonPressed; }
		virtual constexpr EventType getEventType() const override { return EventType::MouseButtonPressed; }

		std::string toString() const override
		{
			return fmt::format("MouseButtonPressedEvent (Button: {})", m_button);
		}
	};


	class MouseButtonReleasedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonReleasedEvent(int button)
			: MouseButtonEvent(button)
		{
		}

		static constexpr EventType getStaticType() { return EventType::MouseButtonReleased; }
		virtual constexpr EventType getEventType() const override { return EventType::MouseButtonReleased; }

		std::string toString() const override
		{
			return fmt::format("MouseButtonRelease (Button: {})", m_button);
		}
	};


	class MouseMovedEvent : public Event
	{
	public:
		MouseMovedEvent(float x, float y)
			: m_posX(x), m_posY(y)
		{
		}

		float getPosX() const { return m_posX; }
		float getPosY() const { return m_posY; }
		std::pair<float, float> getPos() const { return { m_posX, m_posY }; }

		virtual constexpr int getCategoryFlag() const override
		{
			return EventCategory::EventCategoryInput | EventCategory::EventCategoryMouse;
		}

		static constexpr EventType getStaticType() { return EventType::MouseMoved; }
		virtual constexpr EventType getEventType() const override { return EventType::MouseMoved; }

		std::string toString() const override 
		{
			return fmt::format("MouseMovedEvent (Pos: ({}, {}))", m_posX, m_posY);
		}
	private:
		float m_posX, m_posY;
	};


	class MouseScrolledEvent : public Event
	{
	public:
		MouseScrolledEvent(float offsetX, float offsetY)
			: m_offsetX(offsetX), m_offsetY(offsetY)
		{
		}

		float getOffsetX() const { return m_offsetX; }
		float getOffsetY() const { return m_offsetY; }
		std::pair<float, float> getOffset() const { return { m_offsetX, m_offsetY }; }

		virtual constexpr int getCategoryFlag() const override 
		{
			return EventCategory::EventCategoryInput | EventCategory::EventCategoryMouse;
		}

		static constexpr EventType getStaticType() { return EventType::MouseScrolled; }
		virtual constexpr EventType getEventType() const override { return EventType::MouseScrolled; }

		std::string toString() const override
		{
			return fmt::format("MouseScrolledEvent (Offset: ({}, {}))", m_offsetX, m_offsetY);
		}

	private:
		float m_offsetX, m_offsetY;
	};
}