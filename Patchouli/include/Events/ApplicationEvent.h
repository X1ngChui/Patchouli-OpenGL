#pragma once

#include "Event.h"

namespace Pache {
	class AppTickEvent : public Event
	{
	public:
		AppTickEvent() {}

		virtual constexpr int getCategoryFlag() const override
		{
			return EventCategory::EventCategoryApp;
		}

		static constexpr EventType getStaticType() { return EventType::AppTick; }
		virtual constexpr EventType getEventType() const override { return EventType::AppTick; }

		std::string toString() const override
		{
			return "AppTickEvent";
		}
	};


	class AppUpdateEvent : public Event {
	public:
		AppUpdateEvent() {}

		virtual constexpr int getCategoryFlag() const override 
		{
			return EventCategory::EventCategoryApp;
		}

		static constexpr EventType getStaticType() { return EventType::AppUpdate; }
		virtual constexpr EventType getEventType() const override { return EventType::AppUpdate; }

		std::string toString() const override
		{
			return "AppUpdateEvent";
		}
	};


	class AppRenderEvent : public Event 
	{
	public:
		AppRenderEvent() {}

		virtual constexpr int getCategoryFlag() const override 
		{
			return EventCategory::EventCategoryApp;
		}

		static constexpr EventType getStaticType() { return EventType::AppUpdate; }
		virtual constexpr EventType getEventType() const override { return EventType::AppUpdate; }

		std::string toString() const override
		{
			return "AppUpdateEvent";
		}
	};
}