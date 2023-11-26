#pragma once
#include "Events/Event.h"

namespace Pache
{
	class EventQueue
	{
	public:
		EventQueue() = default;
		~EventQueue() = default;

		void push(Event* e);
		Event* pop();
		bool empty();

	private:
		std::queue<Event*> eventQueue;
		std::mutex mutex;
	};
}