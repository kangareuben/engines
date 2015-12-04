#include "Errors.h"
#include "iostream"
#include <cstdlib>
#include "string"
#include <SDL2/SDL.h>
#include <GL/glew.h>
using namespace std;
void fatalError(string errorString)
{
	cout << errorString << endl;
	cout << "Enter any key to quit...";
	int tmp;
	cin >> tmp;
	SDL_Quit();
	exit(1);

}
