#include <iostream>
#include "BaseApplication.h"

using namespace std;
using namespace sg::utility;
using namespace sg::core;
using namespace sg::event_control;
using namespace sg::math;
class Game : public BaseApplication
{
public:
	Game(Window& wnd) : BaseApplication(wnd){}
private:
	void MainGameProccess()
	{

	}
};
int main()
{
	
	Window app(WindowSetting(L"ױוככמף ֲמנכה!", 1000, 600));
	Game g(app);

	return g.Run();
}