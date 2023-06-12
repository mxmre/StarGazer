#include "Window.h"
#include "Crush.h"
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
        if (this->pRender_ != nullptr)
        {
            this->m_app_info_logger.Print(L"Window start...");
            
            this->pWindowThread_ = new std::thread(&Window::WindowProccessRun, this);
            this->m_app_info_logger.Print(L"Window start success.");
        }
        else
        {
            this->m_app_warn_logger.Print(L"Render not binded to window");
        }
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
Window::Window(const std::wstring& window_class, const WindowSetting& wnd) : windowSetting(wnd), isRunning_(false), isClosed_(false), isWindowRunInterrupt_(false),
    pWindowThread_{ nullptr }, pRender_{ nullptr }, _window_class_name(window_class),
    m_app_info_logger(ILogger::LogType::Info),
    m_app_warn_logger(ILogger::LogType::Warning),
    m_app_error_logger(ILogger::LogType::Error)
{
    
}
int Window::WindowProccessRun()
{
    this->m_app_info_logger.Print(L"Window init...");
    Logger<wchar_t>::Info.Print(L"Window class create...");
    WNDCLASS wc = { 0 };

    wc.lpfnWndProc = Window::WindowMessageHandler;
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.hCursor = LoadCursor(0, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)GetStockObject(static_cast<int>(this->windowSetting.backgroundColor_));
    wc.lpszClassName = _window_class_name.c_str();

    Logger<wchar_t>::Info.Print(L"Window class register...");
    
    ATOM reg_result = RegisterClass(&wc);
    sg::exceptions::FatalErrorAssert(!(!reg_result && GetLastError() == ERROR_CLASS_ALREADY_EXISTS), L"Window \"" +
        _window_class_name + L"\" already exist!", sg::exceptions::e_crush_code::WindowClassAlreadyExist);
    Logger<wchar_t>::Info.Print(L"Window create...");
    // Create the window.
    HWND hwnd = CreateWindowEx(
        0,                              // Optional window styles.
        _window_class_name.c_str(),                     // Window class
        this->windowSetting.windowName_.c_str(),    // Window text
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,            // Window style

        // Size and position
        this->windowSetting.x, this->windowSetting.y,
        this->windowSetting.w, this->windowSetting.h,

        NULL,       // Parent window    
        NULL,       // Menu
        NULL,  // Instance handle
        NULL        // Additional application data
    );
    if (hwnd == NULL)
    {
        Logger<wchar_t>::Info.Print(L"Window create failed!");
        return 0;
    }
    Logger<wchar_t>::Info.Print(L"Window create success!");
    this->windowSetting.pWindow_ = hwnd;

    Logger<wchar_t>::Info.Print(L"Window init success!");
    // Run the message loop.
    ShowWindow(hwnd, SW_SHOWNORMAL);
    Logger<wchar_t>::Info.Print(L"Showed window.");
    Logger<wchar_t>::Info.Print(L"Start getting messages from window.");
    MSG msg = { };
    this->isRunning_ = true;
    /*BOOL bRet;*/
    Window::windowIsInit.notify_one();
    //while ((bRet = GetMessage(&msg, this->windowSetting.pWindow_, 0, 0)) != 0)
    while (msg.message != WM_QUIT)
    {
        std::this_thread::sleep_for(5ms);
        if (!this->isRunning_)
        {
            
            this->isWindowRunInterrupt_ = true;
            Logger<wchar_t>::Info.Print(L"Window proccess interrupted.");
            break;
        }
        else
        {
            //if (bRet == -1)
            //{
            //    // handle the error and possibly exit
            //    break;
            //}
            
            if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);

                Event* pEvent = Window::PopEvent();
                if (pEvent != nullptr)
                {
                    inputManager.EventListen(pEvent);
                    delete pEvent;
                }
            }
        }
    }
    //std::unique_lock<std::mutex> locker(Window::_windowMutex);
    
    
    Logger<wchar_t>::Info.Print(L"Window's handle message proccess ends.");
    if (this->isWindowRunInterrupt_) 
    {
        Logger<wchar_t>::Info.Print(L"Window's proccess interrupted.");
        Window::windowProccessIsInterrupted_.notify_one();
    }
    this->Close();
    
    return 0;
}
void Window::JoinWindowThread()
{
    if (this->pWindowThread_ != nullptr && this->pWindowThread_->joinable())
    {
        this->pWindowThread_->join();
    }
        
}
void Window::BindRender(graphics::Render* pRender)
{
    this->pRender_ = pRender;
    Logger<wchar_t>::Info.Print(L"Render binded to window.");
    if (!this->pRender_)
        Logger<wchar_t>::Warn.Print(L"Render pointer is nullptr");
}
void Window::Close()
{
    if (!this->isClosed_)
    {
        this->isClosed_ = true;
        this->isRunning_ = false;
        this->m_app_info_logger.Print(L"Window close...");
        
        if (this->isWindowRunInterrupt_)
        {
            std::unique_lock<std::mutex> locker(Window::windowMutex);
            Window::windowProccessIsInterrupted_.wait(locker);
        }
    }
}
Window::~Window()
{
    this->Close();
    _DELETE(this->pWindowThread_);
}

std::mutex Window::windowMutex;
std::condition_variable Window::windowProccessIsInterrupted_, Window::windowIsInit;