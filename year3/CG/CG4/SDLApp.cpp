#include "libs.h"
#include "SDLApp.h"



SDLApp::SDLApp() : program(inputstate)
{}

void SDLApp::run()
{
	bool running = true;

	if (!createWindow())
		return;

	program.initialize();

	Uint32 lasttime = SDL_GetTicks();
	Uint32 newtime;

	while (running)
	{
		handleEvents(running);

		newtime = SDL_GetTicks();
		program.update(width, height, (newtime - lasttime) * 0.001);
		lasttime = newtime;

		program.render();

		SDL_GL_SwapBuffers();
	}

	cleanUp();
}

bool SDLApp::createWindow()
{
	int i;

	//initialize SDL for OpenGL
	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
        return false;
    }

	width = 800;
	height = 600;

	for (i = 0; i < SDLK_LAST; ++i)
	{
		inputstate.keyspress[i] = false;
		inputstate.keysdown[i] = false;
	}

	//set depth buffer to 24 bits for better precision
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	//set window title
	SDL_WM_SetCaption("Computer Graphics Workshop 2: Mountains", NULL);
 
	//create a window
	if((screensurface = SDL_SetVideoMode(width, height, 32, SDL_HWSURFACE | SDL_GL_DOUBLEBUFFER | SDL_OPENGL | SDL_RESIZABLE)) == NULL)
	{
        return false;
    }

	if (glewInit() != GLEW_OK)
	{
		return false;
	}

	return true;
}

void SDLApp::cleanUp()
{
	SDL_Quit();
}

void SDLApp::handleEvents(bool & running)
{
	int i;
	//reset key press events
	for (i = 0; i < SDLK_LAST; ++i)
		inputstate.keyspress[i] = false;

	SDL_Event sdlevent;
	while(SDL_PollEvent(&sdlevent))
	{
		if (sdlevent.type == SDL_QUIT)
		{
			running = false;
		}
		else if (sdlevent.type == SDL_KEYDOWN)
		{
			if (sdlevent.key.keysym.sym == SDLK_ESCAPE)
			{
				running = false;
			}
			else
			{
				inputstate.keyspress[sdlevent.key.keysym.sym] = true;
				inputstate.keysdown[sdlevent.key.keysym.sym] = true;
			}
		}
		else if (sdlevent.type == SDL_KEYUP)
		{
			inputstate.keysdown[sdlevent.key.keysym.sym] = false;
		}
		else if (sdlevent.type == SDL_VIDEORESIZE)
		{
			width = sdlevent.resize.w;
			height = sdlevent.resize.h;
		}
	}
}
