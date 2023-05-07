#include <iostream>
#include "Application.h"

using namespace std;
using namespace sg::utility;
using namespace sg::core;
using namespace sg::event_control;
int main()
{
	sg::utility::Logger<char> logger(ILogger::LogType::Info, ILogger::LogFlags::StdOutput | ILogger::LogFlags::Timed);
	Window* window = new Window(L"Õåëëîó Âîðëä!", 10, 10, 640, 480);
	Application app(window);
	app.Run();
	while (app.IsRunning())
	{
		Event* ev = Application::PopEvent();
		if (ev != nullptr)
		{
			switch (ev->event_type)
			{
			case Event::EventType::WindowEvent:
			{
				WindowEvent* window_event = (WindowEvent*)ev;
				logger.Print("WindowEvent catched");
				switch (window_event->window_event_type)
				{
				case WindowEvent::WindowEventType::WindowDestroy:
					
					app.Close();
					break;
				default:
					break;
				}
				break;
			}
			case Event::EventType::KeyEvent:
			{
				
				KeyEvent* key_event = (KeyEvent*)ev;

				logger.Print("Key pressed: " + std::to_string(static_cast<int>(key_event->key_code)));
				switch (key_event->key_code)
				{
				default:
					break;
				}
				break;
			}
			default:
				break;
			}
			Application::DeleteEvent(ev);
		}
		
	}
	return 0;
}