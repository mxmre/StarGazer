#pragma once
#include "stdafx.h"
#include "Events.h"

namespace sg
{
	namespace event_control
	{
		class EventQueue
		{
		public:
			EventQueue();

			void Push(Event* const event);
			Event* Pop();
		private:
			std::queue<Event*> m_event_queue;
		};
	}
}


