#pragma once
#include "stdafx.h"
#include "WindowSetting.h"
#include "Logger.h"

namespace sg
{
	namespace core
	{
		class BaseGame;
		class Window
		{
		public:
			Window(const WindowSetting& wnd);
			~Window();
			bool IsRunning() const;
			

			friend class BaseGame;

			static sg::event_control::Event* PopEvent();
			static void DeleteEvent(sg::event_control::Event* ev);
			WindowSetting& GetWindowSettings();
		private:
			void Close();
			bool m_is_running;
			WindowSetting m_window_setting;
			sg::utility::Logger<wchar_t> m_app_info_logger, m_app_warn_logger, m_app_error_logger;

			static int WindowProccessRun(WindowSetting& window);
			static LRESULT WindowProccess(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
			static sg::event_control::EventQueue EVENT_QUEUE;
		};

	}
}

