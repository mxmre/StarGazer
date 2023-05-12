#include "Window.h"
using namespace sg::core;
using namespace sg::utility;
using namespace sg::event_control;
using namespace std;

std::queue<sg::event_control::Event*> Window::EVENT_QUEUE;


sg::event_control::Event* Window::PopEvent()
{
    if (!Window::EVENT_QUEUE.empty())
    {
        Event* ev = Window::EVENT_QUEUE.front();
        Window::EVENT_QUEUE.pop();
        return ev;
    }
        
    else return nullptr;
}
LRESULT CALLBACK Window::WindowMessageHandler(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    using namespace sg::event_control;
    /*sg::utility::Logger<wchar_t> window_info_logger(ILogger::LogType::Info, ILogger::LogFlags::StdOutput | ILogger::LogFlags::Timed);
    sg::utility::Logger<wchar_t> window_warn_logger(ILogger::LogType::Warning, ILogger::LogFlags::StdOutput | ILogger::LogFlags::Timed);
    sg::utility::Logger<wchar_t> window_error_logger(ILogger::LogType::Error, ILogger::LogFlags::StdOutput | ILogger::LogFlags::Timed);*/
    switch (uMsg)
    {
    case WM_DESTROY:
        
        Window::EVENT_QUEUE.push(new WindowEvent(WindowEventType::WindowDestroy));
        PostQuitMessage(0);
        return 0;
    case WM_KEYDOWN:
    {
        KeyCode code = static_cast<KeyCode>(wParam);
        Window::EVENT_QUEUE.push(new KeyEvent(code, true));
        return 0;
    }
    case WM_KEYUP:
    {
        KeyCode code = static_cast<KeyCode>(wParam);
        Window::EVENT_QUEUE.push(new KeyEvent(code, false));
        return 0;
    }
    case WM_MOUSEHOVER:
    {
        
        return 0;
    }
    case WM_MOUSELEAVE:
    {
        sg::math::uPoint2d screen_pos = { LOWORD(wParam), HIWORD(wParam) };
        sg::math::uPoint2d window_pos = { LOWORD(lParam), HIWORD(lParam) };
        Window::EVENT_QUEUE.push(new MouseLeaveEvent(screen_pos, window_pos));
        return 0;
    }
    case WM_MOUSEMOVE:
    {
        sg::math::uPoint2d screen_pos = { LOWORD(wParam), HIWORD(wParam) };
        sg::math::uPoint2d window_pos = { LOWORD(lParam), HIWORD(lParam) };
        Window::EVENT_QUEUE.push(new MouseMoveEvent(screen_pos, window_pos));
        return 0;
    }
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
void Window::Run()
{
    
    if (!this->isRunning_)
    {
        this->m_app_info_logger.Print(L"Window start...");
        this->isRunning_ = true;
        this->windowThread_ = new std::thread(Window::WindowProccessRun, std::ref(*this), std::ref(this->inputManager));
        this->m_app_info_logger.Print(L"Window start success.");
    }
    else
    {
        this->m_app_warn_logger.Print(L"Window already started");
    }
    
}
bool Window::IsRunning() const
{
    return this->isRunning_;
}
Window::Window(const WindowSetting& wnd) : windowSetting(wnd), isRunning_(false), isClosed_(false),
    windowThread_{nullptr},
    m_app_info_logger(ILogger::LogType::Info),
    m_app_warn_logger(ILogger::LogType::Warning),
    m_app_error_logger(ILogger::LogType::Error)
{
    
}
int Window::WindowProccessRun(Window& window, sg::event_control::InputManager& inputManager)
{
    window.m_app_info_logger.Print(L"Window init...");
    Logger<wchar_t>::infoLogger.Print(L"Window class create...");
    WNDCLASS wc = { 0 };

    wc.lpfnWndProc = Window::WindowMessageHandler;
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.hCursor = LoadCursor(0, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)GetStockObject(static_cast<int>(window.windowSetting.backgroundColor_));
    wc.lpszClassName = WindowSetting::WINDOW_CLASS_NAME.c_str();

    Logger<wchar_t>::infoLogger.Print(L"Window class register...");
    RegisterClass(&wc);
    Logger<wchar_t>::infoLogger.Print(L"Window create...");
    // Create the window.
    HWND hwnd = CreateWindowEx(
        0,                              // Optional window styles.
        WindowSetting::WINDOW_CLASS_NAME.c_str(),                     // Window class
        window.windowSetting.windowName_.c_str(),    // Window text
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,            // Window style

        // Size and position
        window.windowSetting.x, window.windowSetting.y,
        window.windowSetting.w, window.windowSetting.h,

        NULL,       // Parent window    
        NULL,       // Menu
        NULL,  // Instance handle
        NULL        // Additional application data
    );
    if (hwnd == NULL)
    {
        Logger<wchar_t>::infoLogger.Print(L"Window create failed!");
        return 0;
    }
    Logger<wchar_t>::infoLogger.Print(L"Window create success!");
    window.windowSetting.windowHandle_ = hwnd;

    Logger<wchar_t>::infoLogger.Print(L"Window init success!");
    // Run the message loop.
    ShowWindow(hwnd, SW_SHOWNORMAL);
    Logger<wchar_t>::infoLogger.Print(L"Showed window.");
    Logger<wchar_t>::infoLogger.Print(L"Start getting messages from window.");
    MSG msg = { };

    while (msg.message != WM_QUIT)
    {
        Event* pEvent = Window::PopEvent();
        if (pEvent != nullptr)
        {
            inputManager.EventListen(pEvent);
            Logger<wchar_t>::infoLogger.Print(L"Event delete...");
            delete pEvent;
            Logger<wchar_t>::infoLogger.Print(L"Event deleted.");
        }

        if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
    window.isRunning_ = false;
    Logger<wchar_t>::infoLogger.Print(L"Window's handle message proccess ends.");
    return 0;
}
void Window::JoinWindowThread()
{
    if (this->windowThread_ != nullptr && this->windowThread_->joinable())
    {
        this->windowThread_->join();
    }
        
}
void Window::Close()
{
    if (!this->isClosed_)
    {
        this->m_app_info_logger.Print(L"Window close...");
        if (this->windowThread_) delete this->windowThread_;
        //if (this->windowSetting.windowHandle_) CloseWindow(this->windowSetting.windowHandle_);
        this->m_app_info_logger.Print(L"Window closed");
        this->isClosed_ = true;
    }
    
}
Window::~Window()
{
    this->Close();
}