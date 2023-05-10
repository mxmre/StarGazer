#include "InputControl.h"
using namespace sg::event_control;
using namespace sg::math;
using namespace sg::utility;

sg::event_control::InputControl::InputControl() : m_mouse_position(uPoint2d{ 0,0 }, uPoint2d{ 0,0 }), m_mouse_in_window_rect(false),
	m_input_control_info_logger(ILogger::LogType::Info)
{
	this->m_keys_states.fill(false);

}

void sg::event_control::InputControl::KeyInputListen(Event* ev)
{
	KeyEvent* key_ev = reinterpret_cast<KeyEvent*>(ev);
	bool new_key_state = false;
	if (key_ev->key_down) new_key_state = true;
	this->m_keys_states.at(static_cast<size_t>(key_ev->key_code)) = new_key_state;
}

void sg::event_control::InputControl::MouseMoveListen(Event* ev)
{
	MouseEvent* mouse_ev = reinterpret_cast<MouseMoveEvent*>(ev);
	switch (mouse_ev->mouse_event_type)
	{
	case MouseEventType::MouseMove:
		this->m_mouse_in_window_rect = true;
		break;
	case MouseEventType::MouseLeave:
		this->m_mouse_in_window_rect = false;
		break;
	case MouseEventType::MouseHower:
		this->m_mouse_in_window_rect = true;
		break;
	default:
		break;
	}
	
	this->m_mouse_position = MousePosition(*mouse_ev);
	this->m_input_control_info_logger.Print("Mouse coord: " + std::to_string(this->m_mouse_position.window_pos.x) +
		"; " + std::to_string(this->m_mouse_position.window_pos.y));
}

void sg::event_control::InputControl::EventListen(Event* ev)
{
	if (ev != nullptr)
	{
		switch (ev->event_type)
		{
		case EventType::KeyEvent:
			this->KeyInputListen(ev);
			break;
		case EventType::MouseEvent:
			this->MouseMoveListen(ev);
			break;
		default:
			break;
		}
	}
}

bool sg::event_control::InputControl::KeyIsDown(const KeyCode key_code) const
{
	return this->m_keys_states.at(static_cast<size_t>(key_code));
}

bool sg::event_control::InputControl::MouseInWindowRect() const
{
	return this->m_mouse_in_window_rect;
}

MousePosition sg::event_control::InputControl::GetMousePosition() const
{
	return this->m_mouse_position;
}
