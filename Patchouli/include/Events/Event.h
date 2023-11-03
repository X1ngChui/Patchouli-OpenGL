#pragma once
#include "Patchoulipch.h"
#include "spdlog/fmt/fmt.h"

#define PACHE_BIND_EVENT_CALLBACK(function) std::bind(&function, this, std::placeholders::_1)

namespace Pache
{
	// The Value of EventType is Its Priority
	enum class EventType
	{
		None = 0,
		WindowClosed, WindowMoved, WindowResize, WindowFocus, WindowLostFucous,
		AppTick, AppUpdate, AppRender,
		KeyPressed, KeyReleased, KeyTyped,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
	};

	enum EventCategory
	{
		None						= 0x00000000,
		EventCategoryApp			= 0x00000001,
		EventCategoryWindow			= 0x00000002,
		EventCategoryInput			= 0x00000004,
		EventCategoryKeyboard		= 0x00000008,
		EventCategoryMouse			= 0x00000010,
		EventCategoryMouseButton	= 0x00000020
	};

	class Event
	{
		friend class EventDispatcher;
	public:
		virtual constexpr EventType getEventType() const = 0;
		virtual constexpr int getCategoryFlag() const = 0;
		virtual std::string toString() const = 0;

		bool belongToCategory(EventCategory category) { return getCategoryFlag() & category; }
	private:
		bool dealt = false;
	};
	
	class EventDispatcher
	{
	public:
		EventDispatcher(Event& evt)
			: m_Event(evt)
		{
		}

		template <typename E>
		bool dispatch(const std::function<bool(E&)> handler)
		{
			if (m_Event.getEventType() == E::getStaticType())
			{
				m_Event.dealt |= handler(static_cast<E&>(m_Event));
				return true;
			}
			return false;
		}
	private:
		Event& m_Event;
	};
}

template <typename T>
struct fmt::formatter<T, std::enable_if_t<std::is_base_of_v<Pache::Event, T>, char>>
	:fmt::formatter<std::string> 
{
	template <typename FormatCtx>
	auto format(const Pache::Event& evt, FormatCtx& ctx) const
	{
		return fmt::formatter<std::string>::format(evt.toString(), ctx);
	}
};
