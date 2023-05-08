#pragma once
#include "Window.h"
#include "InputControl.h"
namespace sg
{
	namespace core
	{
		class BaseGame
		{
		public:
			BaseGame(Window& main_window);
			int Run();

		protected:
			virtual void MainGameProccess() = 0;
			Window& m_main_window;
			sg::utility::Logger<wchar_t> m_game_info_logger, m_game_warn_logger, m_game_error_logger;
			sg::event_control::InputControl m_input_control;
		};
	}
}