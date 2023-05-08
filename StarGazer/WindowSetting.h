#pragma once
#include <Windows.h>
#include <string>

#include "EventQueue.h"
#include "BasicMathTypes.h"
namespace sg
{
	namespace core
	{
		class Window;
		class WindowSetting
		{
		public:
			WindowSetting(const std::wstring& window_name,
				const sg::math::Rect& window_rect);
			bool IsCreated() const;
			friend class Window;
		private:
			sg::math::Rect m_window_rect;
			std::wstring m_window_name;

			HWND m_hwnd;
			static const std::wstring WINDOW_CLASS_NAME;
		};
	}
}


