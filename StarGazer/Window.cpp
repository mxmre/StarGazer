#include "Window.h"
using namespace sg::core;

const std::wstring Window::WINDOW_CLASS_NAME = L"STARGAZER_DEF_WINDOW_CLASS";

Window::Window(const std::wstring& window_name,
	uint32_t pos_x, uint32_t pos_y,
	uint32_t width, uint32_t height) : m_window_name(window_name),
	m_pos_x(pos_x), m_pos_y(pos_y), m_height(height), m_width(width),
	m_hwnd(nullptr)
{

}

bool Window::IsCreated() const
{
	return this->m_hwnd != nullptr;
}