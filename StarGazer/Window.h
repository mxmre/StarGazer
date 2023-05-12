#pragma once
#include "stdafx.h"
#include "Events.h"
#include "WindowSetting.h"
#include "InputManager.h"
#include "Logger.h"

namespace sg
{
	namespace core
	{
		class BaseApplication;
		class Window
		{
		public:
			Window(const WindowSetting& wnd);
			~Window();
			bool IsRunning() const;
			void Run();

			friend class BaseApplication;
			
			static sg::event_control::Event* PopEvent();
			void JoinWindowThread();
			sg::event_control::InputManager inputManager;
			WindowSetting windowSetting;
		private:
			void Close();
			
			static int WindowProccessRun(Window& window, sg::event_control::InputManager& inputManager);
			static LRESULT WindowMessageHandler(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
			static std::queue<sg::event_control::Event*> EVENT_QUEUE;

			bool isRunning_, isClosed_;
			
			std::thread* windowThread_;

			sg::utility::Logger<wchar_t> m_app_info_logger, m_app_warn_logger, m_app_error_logger;
		};

	}
}

