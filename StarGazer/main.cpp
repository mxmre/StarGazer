#include <iostream>
#include "BaseGame.h"

using namespace std;
using namespace sg::utility;
using namespace sg::core;
using namespace sg::event_control;
using namespace sg::math;
class Game : public BaseGame
{
public:
	Game(Window& wnd) : BaseGame(wnd){}
private:
	void MainGameProccess()
	{

	}
};
int main()
{
	
	Window app(WindowSetting(L"������ �����!", Rect{ 10, 10, 640, 480 }));
	Game g(app);

	return g.Run();
}