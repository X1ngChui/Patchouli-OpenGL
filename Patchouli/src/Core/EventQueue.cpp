#include "Core/EventQueue.h"

namespace Pache
{
	void EventQueue::enqueue(Event* e)
	{
		std::lock_guard<std::mutex> lock(mutex);
		eventQueue.push(e);
		queueNotEmpty.notify_one();
	}

	void EventQueue::processEvent()
	{
		std::unique_lock<std::mutex> lock(mutex);
		queueNotEmpty.wait(lock, [this]() { return !(this->eventQueue).empty(); });

		while (!eventQueue.empty())
		{
			Event* event = eventQueue.front();
			eventQueue.pop();
			onEvent(*event);

			// TODO: Implement object pooling for Event objects to optimize memory allocation.
			// Currently using raw malloc and raw delete. Consider using a custom object pool
			// to reuse memory blocks for Event instances and reduce the overhead of frequent
			// allocations and deallocations.
			delete event;
		}
	}
}