#pragma once
#include "stdafx.h"

#include "Logger.h"
#include "Events.h"
namespace sg
{
	namespace event_control
	{
		class InputControl
		{
		public:

			InputControl();
			
			void EventListen(Event* ev);

			bool KeyIsDown(const KeyCode key_code) const;
			bool MouseInWindowRect() const;
			MousePosition GetMousePosition() const;
		private:
			MousePosition m_mouse_position;
			
			bool m_mouse_in_window_rect;
			static const size_t KEYS_STATES_SIZE = 0x100;
			std::array<bool, InputControl::KEYS_STATES_SIZE> m_keys_states;

			void KeyInputListen(Event* ev);
			void MouseMoveListen(Event* ev);

			sg::utility::Logger<char> m_input_control_info_logger;
		};
	}
}


