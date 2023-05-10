#pragma once
#include "stdafx.h"

#include "utility_objects.h"
#include "EventQueue.h"
#include "BasicMathTypes.h"
namespace sg
{
	namespace core
	{
		class Window;
		/*enum class CursorStyle
		{
			Arrow = 0,
			Cross,
			Hand,
			No,
			Wait,
			SizeAll,
			SizeURDL,
			SizeULDR,
			SizeLR,
			SizeUD
		};*/
		enum class WindowScreenMode
		{
			Window = 0,
			Fullscreen
		};
		enum class WindowBackgroundColor
		{
			Black = BLACK_BRUSH,
			White = WHITE_BRUSH,
			Gray  = DKGRAY_BRUSH
		};
		class WindowSetting : public sg::utility::RectangularObject<uint32_t>
		{
		public:
			WindowSetting(const std::wstring& window_name,
				const uint32_t w, const uint32_t h, const WindowBackgroundColor bg_color = WindowBackgroundColor::Gray,
				bool cursor_is_visible = true);
			bool IsWindowCreated() const;

			friend class Window;

			void ShowCursor();
			void HideCursor();
			bool CursorIsVisible() const;
			const HWND Handle() const;
			void SetResolution(const uint32_t w, const uint32_t h);
		private:
			bool m_cursor_is_visible;
			std::wstring m_window_name;
			WindowBackgroundColor m_bg_color;
			/*CursorStyle m_cursor_style;*/
			HWND m_hwnd;
			static const std::wstring WINDOW_CLASS_NAME;
		};
	}
}


