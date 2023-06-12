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
int foo0()
{
	while (!ThreadPool::thread_is_interrupted())
	{
		//std::cout << "0";
	}
	return 1;
}
int foo1()
{
	return 2;
}
int foo2()
{
	while (!ThreadPool::thread_is_interrupted())
	{
	}
	return 3;
}
int main()
{
	int mainResult = 0;
	std::map<int, int> a;
	a[2] = 2;
	a[5] = 2;
	a.erase(2);
	/*Render r;
	Window app(WindowSetting(L"ױוככמף ֲמנכה!", 1000, 600));
	Game g(app, r);
	mainResult = g.Run();*/
	sg::core::ThreadPool tp(3);
	auto res0 = tp.add_task(0, foo0);
	auto res1 = tp.add_task(1, foo1);
	auto res2 = tp.add_task(2, foo2);
//	std::cout << res1.get();
	tp.close_thread(0);
	
	tp.close_thread(1);
	tp.close_thread(0);
	//std::cout << res0.get() << res2.get();
	return mainResult;
}