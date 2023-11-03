#pragma once

#include "Event.h"

namespace Pache
{
	class KeyboardEvent : public Event
	{
	public:
		int getKeyCode() const { return keyCode; }

		virtual constexpr int getCategoryFlag() const override
		{
			return EventCategory::EventCategoryInput | EventCategory::EventCategoryKeyboard;
		}

	protected:
		int keyCode;
		
		KeyboardEvent(int keyCode)
			: keyCode(keyCode) {}
	};


	class KeyPressedEvent : public KeyboardEvent
	{
	public:
		KeyPressedEvent(int keyCode, bool repeated)
			: KeyboardEvent(keyCode), repeated(repeated) {}

		bool isRepeated() const { return repeated; }

		static constexpr EventType getStaticType() { return EventType::KeyPressed; }
		virtual constexpr EventType getEventType() const override { return EventType::KeyPressed; }


		std::string toString() const override
		{
			return fmt::format("KeyPressedEvent (Keycode: {}, Repeated: {})", keyCode, repeated);
		}
	private:
		bool repeated;
	};

	class KeyReleasedEvent : public KeyboardEvent
	{
	public:
		KeyReleasedEvent(int keyCode)
			: KeyboardEvent(keyCode) {}

		static constexpr EventType getStaticType() { return EventType::KeyReleased; }
		virtual constexpr EventType getEventType() const override { return EventType::KeyReleased; }

		std::string toString() const override
		{
			return fmt::format("KeyRelaesedEvent (Keycode: {})", keyCode);
		}
	};

	class KeyTypedEvent : public KeyboardEvent
	{
	public:
		KeyTypedEvent(int keyCode)
			: KeyboardEvent(keyCode) {}

		static constexpr EventType getStaticType() { return EventType::KeyTyped; }
		virtual constexpr EventType getEventType() const override { return EventType::KeyTyped; }

		std::string toString() const override 
		{
			return fmt::format("KeyTypedEvent (Keycode: {})", keyCode);
		}
	};
}