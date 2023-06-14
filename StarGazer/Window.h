#pragma once
#include "stdafx.h"
#include "Events.h"
#include "WindowSetting.h"
#include "InputManager.h"
#include "Logger.h"

namespace sg
{
	namespace graphics
	{
		class Render;
	}
	namespace core
	{

		class Window
		{
		public:
			
			Window(const std::wstring& window_class,const WindowSetting& wnd);
			~Window();
			bool IsRunning() const;
			bool Init();
			void Run();
			void Close();
			
			static sg::event_control::Event* PopEvent();
			sg::event_control::InputManager inputManager;
			WindowSetting windowSetting;
			void BindRender(graphics::Render* pRender);

			static std::mutex windowMutex;
			static std::condition_variable windowIsInit;
		private:
			
			
			int Run_();
			static LRESULT WindowMessageHandler(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
			static std::queue<sg::event_control::Event*> EVENT_QUEUE;
			static std::condition_variable windowProccessIsInterrupted_;

			bool isRunning_;
			sg::graphics::Render* pRender_;
			std::wstring _window_class_name;
			sg::utility::Logger<wchar_t> m_app_info_logger, m_app_warn_logger, m_app_error_logger;
		};

	}
}

