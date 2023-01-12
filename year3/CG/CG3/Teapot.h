#include "InputState.h"
#include "ShaderProgram.h"
#include "geometry.h"
 

class Workshop2
{
public:
	Workshop2(InputState & inputstate);
	//setup data
	bool initialize();
	//called before each render, deltatime is in seconds
	void update(int width, int height, double deltatime);
	//render the scene
	void render();

private:
	InputState & inputstate;

	int width, height;
	float time;
	bool toonshader, diffuseshader;

	Vector3 viewpoint, viewtarget;

	//two buffers to store our vertex data
	GLuint vertexbufferobject_position;
	GLuint vertexbufferobject_normal;

	ShaderProgram terrainshader;

	Vector3 light_position;
	Vector3 light_intensity;

	bool loadTeapot();
};
