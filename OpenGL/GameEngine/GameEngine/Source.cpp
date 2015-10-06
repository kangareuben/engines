/*#include "MainGame.h"
#include "iostream"

#include <boost/thread/thread.hpp>

using namespace std;
ObjLoader obj;
ObjLoader obj2;
ObjLoader obj3;
int _obn1;
int _obn2;
int _obn3;
void fn1()
{
	_obn1 = obj.load("Models/ViolinCase.obj");
	
}
void fn2()
{
	_obn2 = obj2.load("Models/capsule.obj");
	
}
void fn3()
{
	_obn3 = obj3.load("Models/Teapot.obj");
}

int main(int argc, char **argv)
{
	MainGame mainGame;
	boost::thread thrd1(&fn1);
//	boost::thread thrd2(&fn2);
	boost::thread thrd3(&fn3);
	thrd1.join();
//	thrd2.join();
	thrd3.join();
	
	mainGame.run();

	return 0;
}*/