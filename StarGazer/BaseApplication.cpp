#include "BaseApplication.h"
#include "Crush.h"

using namespace sg::core;
using namespace sg::utility;
using namespace sg::event_control;

BaseApplication::BaseApplication(Window& refApplicationWindow, sg::graphics::Render& refRender, uint16_t thread_count)
	: refApplicationWindow_(refApplicationWindow), refRender_(refRender), thread_pool_(thread_count),
	m_game_info_logger(ILogger::LogType::Info),
	m_game_warn_logger(ILogger::LogType::Warning),
	m_game_error_logger(ILogger::LogType::Error)
{
	this->refApplicationWindow_.BindRender(&this->refRender_);
	this->refRender_.BindWindow(&this->refApplicationWindow_);
}
void BaseApplication::Close()
{
	this->thread_pool_.close_thread(0);
}
int BaseApplication::Run()
{
	_SG_TRY_START
	auto WindowInit = this->thread_pool_.add_task(0, &Window::Init, &this->refApplicationWindow_);
	WindowInit.get();
	auto RenderInit = this->thread_pool_.add_task(0, &sg::graphics::Render::Init, &this->refRender_);
	sg::exceptions::ErrorAssert(RenderInit.get(), "Render init error!");
	this->thread_pool_.add_task(0, &Window::Run, &this->refApplicationWindow_);
	graphics::ColorRGBA8 color{ 0, 0, 0, 0 };
	srand(0);
	while (this->refApplicationWindow_.IsRunning())
	{

		this->MainGameProccess();
		this->refRender_.ClearBuffers(color);
		color.a = rand() % 256;
		color.r = rand() % 256;
		color.g = rand() % 256;
		color.b = rand() % 256;
	}
	this->Close();
	return 0;
	_SG_TRY_END
	this->Close();
	return 1;
}