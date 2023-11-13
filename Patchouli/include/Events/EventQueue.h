#pragma once
#include "Events/Event.h"

namespace Pache
{
	class EventQueue
	{
	public:
		using EventCallback = std::function<void(Event&)>;

		EventQueue(EventCallback onEvent)
			: onEvent(onEvent)
		{
		}
		~EventQueue() = default;

		void enqueue(Event* e);

		void processEvent();

	private:
		EventCallback onEvent;
		std::queue<std::unique_ptr<Event>> eventQueue;
		std::mutex mutex;
	};
}