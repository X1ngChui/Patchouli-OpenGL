#include "Events/Event.h"

namespace Pache
{
	void EventQueue::enqueue(Event* e)
	{
		std::lock_guard<std::mutex> lock(mutex);
		eventQueue.push(std::unique_ptr<Event>(e));
		Log::coreInfo(eventQueue.size());
	}

	void EventQueue::processEvent()
	{
		std::unique_lock<std::mutex> lock(mutex);

		while (!eventQueue.empty())
		{
			std::unique_ptr<Event> event = std::move(eventQueue.front());
			eventQueue.pop();

			onEvent(*event);

			// TODO: Implement object pooling for Event objects to optimize memory allocation.
			// Currently using raw malloc and raw free. Consider using a custom object pool to
			// reuse memory blocks for Event instances and reduce the overhead of frequent
			// allocations and deallocations.
		}
	}
}