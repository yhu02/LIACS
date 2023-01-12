#include "workshop4.h"
 

class SDLApp
{
public:
	SDLApp();

	void run();

private:
	Workshop4 program;
	InputState inputstate;

	SDL_Surface * screensurface;

	//window size
	int width, height;

	
	bool createWindow();
	void cleanUp();

	void handleEvents(bool & running);
};
