#pragma once
#include <thread>
#include <chrono>
#include "WindowSetting.h"
#include "Logger.h"

namespace sg
{
	namespace core
	{
		class Window
		{
		public:
			Window(const WindowSetting& wnd);
			~Window();
			void Run();
			bool IsRunning() const;
			void Close();

			static sg::event_control::Event* PopEvent();
			static void DeleteEvent(sg::event_control::Event* ev);
		private:
			bool m_is_running;
			WindowSetting m_window_setting;
			sg::utility::Logger<wchar_t> m_app_info_logger, m_app_warn_logger, m_app_error_logger;

			static int WindowProccessRun(WindowSetting& window);
			static LRESULT WindowProccess(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
			static sg::event_control::EventQueue EVENT_QUEUE;
		};

	}
}

