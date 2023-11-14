#pragma once
#include "Events/Event.h"

namespace Pache
{
	class EventQueue
	{
	public:
		using EventCallback = std::function<void(Event&)>;

		EventQueue() = default;
		~EventQueue() = default;

		void push(Event* e);
		Event* pop();
		bool empty();

	private:
		EventCallback onEvent;
		std::queue<Event*> eventQueue;
		std::mutex mutex;
	};
}