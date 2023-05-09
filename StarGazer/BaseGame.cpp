#include "BaseGame.h"


using namespace sg::core;
using namespace sg::utility;
using namespace sg::event_control;

BaseGame::BaseGame(Window& main_window) : m_main_window(main_window),
	m_game_info_logger(ILogger::LogType::Info),
	m_game_warn_logger(ILogger::LogType::Warning),
	m_game_error_logger(ILogger::LogType::Error)
{
	this->m_main_window.GetWindowSettings().HideCursor();
	ShowCursor(FALSE);
}
void BaseGame::WindowEventListen(Event* ev, std::thread& window_thread)
{
	if (ev != nullptr && ev->event_type == EventType::WindowEvent)
	{
		WindowEvent* w_ev = reinterpret_cast <WindowEvent*> (ev);
		switch (w_ev->window_event_type)
		{
		case WindowEventType::WindowDestroy:
			sg::exceptions::ErrorAssert(window_thread.joinable(), "Window thread is daemon proccess!");
			window_thread.join();
			this->m_main_window.Close();
			break;
		default:
			break;
		}
	}
}
int BaseGame::Run()
{
	try
	{
		this->m_main_window.m_is_running = true;
		std::thread window_thread(Window::WindowProccessRun, std::ref(this->m_main_window.m_window_setting));
		
		while (this->m_main_window.IsRunning())
		{
			Event* ev = Window::PopEvent();

			/*ShowCursor(((this->m_input_control.MouseInWindowRect() && this->m_main_window.GetWindowSettings().CursorIsVisible())
				|| !this->m_input_control.MouseInWindowRect()));*/
			
			this->WindowEventListen(ev, window_thread);
			this->m_input_control.EventListen(ev);
			this->MainGameProccess();

			Window::DeleteEvent(ev);
		}
		return 0;
	}
	catch (sg::exceptions::SGFatalException& err)
	{
		this->m_game_error_logger.Print(err.what());
		return 1;
	}
	catch (sg::exceptions::SGException& err)
	{
		this->m_game_error_logger.Print(err.what());
		return 2;
	}
	catch (std::exception& err)
	{
		std::string err_msg = err.what();
		this->m_game_error_logger.Print(std::wstring(err_msg.begin(), err_msg.end()));
		return 3;
	}
	catch (...)
	{
		this->m_game_error_logger.Print(L"Unknown error!");
		return 4;
	}
}