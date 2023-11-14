#include "Events/Event.h"

namespace Pache
{
	void EventQueue::push(Event* e)
	{
		std::lock_guard<std::mutex> lock(mutex);
		eventQueue.push(e);
	}

	Event* EventQueue::pop()
	{
		std::lock_guard<std::mutex> lock(mutex);
		Event* evt = eventQueue.front();
		eventQueue.pop();
		return evt;
	}

	bool EventQueue::empty()
	{
		return eventQueue.empty();
	}

}