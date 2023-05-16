#include "WindowSetting.h"
using namespace sg::core;

const std::wstring WindowSetting::WINDOW_CLASS_NAME = L"STARGAZER_DEF_WINDOW_CLASS";

WindowSetting::WindowSetting(const std::wstring& window_name,
	const uint32_t w, const uint32_t h,
	const WindowBackgroundColor bg_color,
	bool cursor_is_visible) : windowName_(window_name), sg::math::RectangularObject<uint32_t>( w, h ),
	pWindow_{ nullptr }, backgroundColor_{ bg_color },
	cursorIsVisible_(cursor_is_visible)
{
	
}

bool WindowSetting::IsWindowCreated() const
{
	return this->pWindow_ != nullptr;
}

void sg::core::WindowSetting::ShowCursor()
{
	this->cursorIsVisible_ = true;
}

void sg::core::WindowSetting::HideCursor()
{
	this->cursorIsVisible_ = false;
}

bool sg::core::WindowSetting::CursorIsVisible() const
{
	return this->cursorIsVisible_;
}

const HWND sg::core::WindowSetting::Handle() const
{
	return this->pWindow_;
}

void sg::core::WindowSetting::SetResolution(const uint32_t w, const uint32_t h)
{
}
