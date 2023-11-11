#pragma once
#include "Patchoulipch.h"
#include "spdlog/fmt/fmt.h"

namespace Pache
{
	// Enumeration defining different types of events
	enum class EventType
	{
		None = 0,
		WindowClosed, WindowMoved, WindowResize, WindowFocus, WindowLostFucous,
		AppTick, AppUpdate, AppRender,
		KeyPressed, KeyReleased, KeyTyped,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
	};

	// Bitmask-based enumeration defining event categories
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

	// Abstract base class for events
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

	template <typename T>
	concept IsEvent = std::derived_from<T, Event>;
	
	// Class responsible for dispatching events and handling them
	class EventDispatcher
	{
	public:
		// Constructor taking an event reference to be dispatched
		EventDispatcher(Event& evt)
			: event(evt)
		{
		}

		bool isDealt() { return event.dealt; }

		// Dispatch an event to a handler function
		template <IsEvent E>
		bool dispatch(std::function<bool(E&)> handler)
		{
			if (event.getEventType() == E::getStaticType())
			{
				event.dealt |= handler(static_cast<E&>(event));
				return true;
			}
			return false;
		}
	private:
		Event& event;
	};
}

// Specialization of the formatter for Event types in fmt library
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
