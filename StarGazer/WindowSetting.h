#pragma once
#include "stdafx.h"

#include "BasicMathObjects.h"
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
		class WindowSetting : public sg::math::RectangularObject<uint32_t>
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
			bool cursorIsVisible_;
			std::wstring windowName_;
			WindowBackgroundColor backgroundColor_;
			/*CursorStyle m_cursor_style;*/
			HWND pWindow_;
		};
	}
}


