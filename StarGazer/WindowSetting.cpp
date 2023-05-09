#include "WindowSetting.h"
using namespace sg::core;

const std::wstring WindowSetting::WINDOW_CLASS_NAME = L"STARGAZER_DEF_WINDOW_CLASS";

WindowSetting::WindowSetting(const std::wstring& window_name,
	const uint32_t w, const uint32_t h,
	const WindowBackgroundColor bg_color,
	bool cursor_is_visible) : m_window_name(window_name), m_window_rect{ 0,0, w, h }, m_hwnd{ nullptr }, m_bg_color{ bg_color },
	m_cursor_is_visible(cursor_is_visible)
{
	
}

bool WindowSetting::IsWindowCreated() const
{
	return this->m_hwnd != nullptr;
}

void sg::core::WindowSetting::ShowCursor()
{
	this->m_cursor_is_visible = true;
}

void sg::core::WindowSetting::HideCursor()
{
	this->m_cursor_is_visible = false;
}

bool sg::core::WindowSetting::CursorIsVisible() const
{
	return this->m_cursor_is_visible;
}

void sg::core::WindowSetting::SetResolution(const uint32_t w, const uint32_t h)
{
}
