#include "EventQueue.h"
using namespace sg::event_control;

sg::event_control::EventQueue::EventQueue()
{
}

void sg::event_control::EventQueue::Push(Event* const event)
{
	this->m_event_queue.push(event);
}

Event* sg::event_control::EventQueue::Pop()
{
	Event* result = nullptr;
	if (!this->m_event_queue.empty())
	{
		result = this->m_event_queue.front();
		this->m_event_queue.pop();
	}
	
	return result;
}
