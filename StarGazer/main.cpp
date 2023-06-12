#include <iostream>
#include "BaseApplication.h"
#include "ThreadPool.h"

using namespace std;
using namespace sg::utility;
using namespace sg::core;
using namespace sg::event_control;
using namespace sg::math;
using namespace sg::graphics;
class Game : public sg::core::BaseApplication
{
public:
	Game(Window& refApplicationWindow, sg::graphics::Render& refRender) : BaseApplication(refApplicationWindow, refRender){}
protected:
	void MainGameProccess()
	{

	}
};

int main()
{
	int mainResult = 0;
	Render r;
	Window app(L"test.exe", WindowSetting(L"ױוככמף ֲמנכה!", 1000, 600));
	Game g(app, r);
	mainResult = g.Run();
	return mainResult;
}