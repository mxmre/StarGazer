#include "WindowSetting.h"
using namespace sg::core;

const std::wstring WindowSetting::WINDOW_CLASS_NAME = L"STARGAZER_DEF_WINDOW_CLASS";

WindowSetting::WindowSetting(const std::wstring& window_name,
	const sg::math::Rect& window_rect) : m_window_name(window_name), m_window_rect(window_rect)
{

}

bool WindowSetting::IsCreated() const
{
	return this->m_hwnd != nullptr;
}