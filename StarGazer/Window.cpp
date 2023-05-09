#include "Window.h"
using namespace sg::core;
using namespace sg::utility;

sg::event_control::EventQueue Window::EVENT_QUEUE;


sg::event_control::Event* Window::PopEvent()
{
	return Window::EVENT_QUEUE.Pop();
}
void Window::DeleteEvent(sg::event_control::Event* ev)
{
	if(ev != nullptr)
		delete ev;
}
LRESULT CALLBACK Window::WindowProccess(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    using namespace sg::event_control;
    /*sg::utility::Logger<wchar_t> window_info_logger(ILogger::LogType::Info, ILogger::LogFlags::StdOutput | ILogger::LogFlags::Timed);
    sg::utility::Logger<wchar_t> window_warn_logger(ILogger::LogType::Warning, ILogger::LogFlags::StdOutput | ILogger::LogFlags::Timed);
    sg::utility::Logger<wchar_t> window_error_logger(ILogger::LogType::Error, ILogger::LogFlags::StdOutput | ILogger::LogFlags::Timed);*/
    switch (uMsg)
    {
    case WM_DESTROY:
        
        Window::EVENT_QUEUE.Push(new WindowEvent(WindowEventType::WindowDestroy));
        PostQuitMessage(0);
        return 0;
    case WM_KEYDOWN:
    {
        KeyCode code = static_cast<KeyCode>(wParam);
        Window::EVENT_QUEUE.Push(new KeyEvent(code, true));
        return 0;
    }
    case WM_KEYUP:
    {
        KeyCode code = static_cast<KeyCode>(wParam);
        Window::EVENT_QUEUE.Push(new KeyEvent(code, false));
        return 0;
    }
    case WM_MOUSEHOVER:
    {
        
        return 0;
    }
    case WM_MOUSELEAVE:
    {
        sg::math::Point2d screen_pos = { LOWORD(wParam), HIWORD(wParam) };
        sg::math::Point2d window_pos = { LOWORD(lParam), HIWORD(lParam) };
        Window::EVENT_QUEUE.Push(new MouseLeaveEvent(screen_pos, window_pos));
        return 0;
    }
    case WM_MOUSEMOVE:
    {
        sg::math::Point2d screen_pos = { LOWORD(wParam), HIWORD(wParam) };
        sg::math::Point2d window_pos = { LOWORD(lParam), HIWORD(lParam) };
        Window::EVENT_QUEUE.Push(new MouseMoveEvent(screen_pos, window_pos));
        return 0;
    }
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
bool Window::IsRunning() const
{
    return this->m_is_running;
}
Window::Window(const WindowSetting& wnd) : m_window_setting(wnd), m_is_running(false),
    m_app_info_logger(ILogger::LogType::Info),
    m_app_warn_logger(ILogger::LogType::Warning),
    m_app_error_logger(ILogger::LogType::Error)
{
    this->m_app_info_logger.Print(L"Application init...");
}
WindowSetting& Window::GetWindowSettings()
{
    return this->m_window_setting;
}
int Window::WindowProccessRun(WindowSetting& window)
{
    sg::utility::Logger<wchar_t> thread_run_info_logger(ILogger::LogType::Info);
    sg::utility::Logger<wchar_t> thread_run_warn_logger(ILogger::LogType::Warning);
    sg::utility::Logger<wchar_t> thread_run_error_logger(ILogger::LogType::Error);
    if (true)
    {
        thread_run_info_logger.Print(L"Window class create...");
        WNDCLASS wc = { 0 };

        wc.lpfnWndProc = Window::WindowProccess;
        wc.style = CS_HREDRAW | CS_VREDRAW;
        wc.hCursor = LoadCursor(0, IDC_ARROW);
        wc.hbrBackground = (HBRUSH)GetStockObject(static_cast<int>(window.m_bg_color));
        wc.lpszClassName = WindowSetting::WINDOW_CLASS_NAME.c_str();

        thread_run_info_logger.Print(L"Window class register...");
        RegisterClass(&wc);
        thread_run_info_logger.Print(L"Window create...");
        // Create the window.
        HWND hwnd = CreateWindowEx(
            0,                              // Optional window styles.
            WindowSetting::WINDOW_CLASS_NAME.c_str(),                     // Window class
            window.m_window_name.c_str(),    // Window text
            WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,            // Window style

            // Size and position
            window.m_window_rect.x, window.m_window_rect.y, window.m_window_rect.w, window.m_window_rect.h,

            NULL,       // Parent window    
            NULL,       // Menu
            NULL,  // Instance handle
            NULL        // Additional application data
        );
        if (hwnd == NULL)
        {
            thread_run_error_logger.Print(L"Window create failed!");
            return 0;
        }
        thread_run_info_logger.Print(L"Window create success!");
        window.m_hwnd = hwnd;
        ShowWindow(hwnd, SW_SHOWNORMAL);
        thread_run_info_logger.Print(L"Application running success!");
        // Run the message loop.
        thread_run_info_logger.Print(L"Start getting messages from window.");
        MSG msg = { };
        while (GetMessage(&msg, NULL, 0, 0) > 0)
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        thread_run_info_logger.Print(L"Window close.");
    }
    return 0;
}

void Window::Close()
{
    if (this->IsRunning())
    {
        this->m_app_info_logger.Print(L"Application close...");
        this->m_is_running = false;
    }
}
Window::~Window()
{
    this->Close();
}