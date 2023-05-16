#include "BaseApplication.h"


using namespace sg::core;
using namespace sg::utility;
using namespace sg::event_control;

BaseApplication::BaseApplication(Window& refApplicationWindow, sg::graphics::Render& refRender)
	: refApplicationWindow_(refApplicationWindow), refRender_(refRender),
	m_game_info_logger(ILogger::LogType::Info),
	m_game_warn_logger(ILogger::LogType::Warning),
	m_game_error_logger(ILogger::LogType::Error)
{
	this->refApplicationWindow_.BindRender(&this->refRender_);
	this->refRender_.BindWindow(&this->refApplicationWindow_);
}
void BaseApplication::Close()
{
	this->refApplicationWindow_.Close();
	
	this->refApplicationWindow_.JoinWindowThread();
}
int BaseApplication::Run()
{
	try
	{
		this->refApplicationWindow_.Run();
		{
			std::unique_lock<std::mutex> locker(Window::windowMutex);
			Window::windowIsInit.wait(locker);
		}
		graphics::ColorRGBA8 color{0, 0, 0, 0};

		//while (!this->refApplicationWindow_.IsRunning());
		sg::exceptions::ErrorAssert(this->refRender_.Init(), "Render init error!");
		while (this->refApplicationWindow_.IsRunning())
		{
			if (color.g == 0xff) 
			{
				color.g = 0;
			}
			
			
			this->MainGameProccess();
			this->refRender_.ClearBuffers(color);
			++color.g;
		}
		this->Close();
		return 0;
	}
	catch (sg::exceptions::SGFatalException& err)
	{
		this->Close();
		this->m_game_error_logger.Print(err.what());
		return 1;
	}
	catch (sg::exceptions::SGException& err)
	{
		this->Close();
		this->m_game_error_logger.Print(err.what());
		return 2;
	}
	catch (std::exception& err)
	{
		this->Close();
		std::string err_msg = err.what();
		this->m_game_error_logger.Print(std::wstring(err_msg.begin(), err_msg.end()));
		return 3;
	}
	catch (...)
	{
		this->Close();
		this->m_game_error_logger.Print(L"Unknown error!");
		return 4;
	}
}