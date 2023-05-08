#pragma once
#include <Windows.h>
#include <string>

#include "EventQueue.h"
namespace sg
{
	namespace core
	{
		class Window;
		class WindowSetting
		{
		public:
			WindowSetting(const std::wstring& window_name,
				uint32_t pos_x, uint32_t pos_y,
				uint32_t width, uint32_t height);
			bool IsCreated() const;
			friend class Window;
		private:
			uint32_t m_width, m_height, m_pos_x, m_pos_y;
			std::wstring m_window_name;

			HWND m_hwnd;
			static const std::wstring WINDOW_CLASS_NAME;
		};
	}
}


