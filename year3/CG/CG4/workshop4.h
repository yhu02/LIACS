#include "InputState.h"
#include "ShaderProgram.h"
#include "geometry.h"
 

class Workshop4
{
public:
	Workshop4(InputState & inputstate);
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
    bool watershader = false;

	Vector3 viewpoint, viewtarget;

	//four buffers to store our vertex data
	GLuint vertexbufferobject_teapot_position;
	GLuint vertexbufferobject_teapot_normal;

	GLuint vertexbufferobject_terrain_position;
	GLuint vertexbufferobject_terrain_normal;

	ShaderProgram terrainshader, teapotshader;

	Vector3 light_position;
	Vector3 light_intensity;

	bool loadTeapot(float scale, float x, float y, float z);
	bool loadTerrain();
	Vector3 getTerrainVertex(std::vector<unsigned char> & heightmap, size_t x, size_t y);
	Vector3 getTerrainNormal(std::vector<unsigned char> & heightmap, size_t x, size_t y);
};
