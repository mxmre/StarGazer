#include "BaseGame.h"


using namespace sg::core;
using namespace sg::utility;
using namespace sg::event_control;

BaseGame::BaseGame(Window& main_window) : m_main_window(main_window),
	m_game_info_logger(ILogger::LogType::Info),
	m_game_warn_logger(ILogger::LogType::Warning),
	m_game_error_logger(ILogger::LogType::Error)
{

}
int BaseGame::Run()
{
	try
	{
		this->m_main_window.Run();
		while (this->m_main_window.IsRunning())
		{
			Event* ev = Window::PopEvent();
			if (ev != nullptr && ev->event_type == EventType::WindowEvent)
			{
				WindowEvent* w_ev = reinterpret_cast <WindowEvent*> (ev);
				switch (w_ev->window_event_type)
				{
				case WindowEventType::WindowDestroy:
					this->m_main_window.Close();
					break;
				default:
					break;
				}
			}


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