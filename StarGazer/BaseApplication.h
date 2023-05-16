#pragma once
#include "stdafx.h"
#include "Window.h"
#include "Render.h"
#include "InputManager.h"
namespace sg
{
	namespace core
	{
		class BaseApplication
		{
		public:
			BaseApplication(Window& refApplicationWindow, sg::graphics::Render& refRender);
			int Run();

		protected:
			void Close();
			virtual void MainGameProccess() = 0;
			Window& refApplicationWindow_;
			sg::graphics::Render& refRender_;
			sg::utility::Logger<wchar_t> m_game_info_logger, m_game_warn_logger, m_game_error_logger;
		};
	}
}