#include "BaseApplication.h"


using namespace sg::core;
using namespace sg::utility;
using namespace sg::event_control;

BaseApplication::BaseApplication(Window& main_window) : appWindow(main_window),
	m_game_info_logger(ILogger::LogType::Info),
	m_game_warn_logger(ILogger::LogType::Warning),
	m_game_error_logger(ILogger::LogType::Error)
{
	
}
void BaseApplication::Close()
{
	this->appWindow.JoinWindowThread();
}
int BaseApplication::Run()
{
	try
	{
		this->appWindow.Run();
		
		while (this->appWindow.IsRunning())
		{
			this->MainGameProccess();
		}
		this->Close();
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