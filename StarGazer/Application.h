#pragma once
#include <thread>
#include <chrono>
#include "Window.h"
#include "Logger.h"

namespace sg
{
	namespace core
	{
		class Application
		{
		public:
			Application(Window* wnd);
			~Application();
			void Run();
			bool IsRunning() const;
			void Close();

			static sg::event_control::Event* PopEvent();
			static void DeleteEvent(sg::event_control::Event* ev);
		private:
			bool m_is_running;
			Window* m_wnd;
			sg::utility::Logger<wchar_t> m_app_info_logger, m_app_warn_logger, m_app_error_logger;

			static int ThreadRun(Window* window);
			static LRESULT WindowProccess(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
			static sg::event_control::EventQueue EVENT_QUEUE;
		};

	}
}

