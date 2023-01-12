#include "libs.h"
#include "Teapot.h"
#include "TeapotModel.h"

//Wireframe mode state
bool wireFrameMode = false;
//Camera turn speed
const float turnSpeed = 1.0;

Workshop2::Workshop2(InputState & inputstate) : inputstate(inputstate)
{}

bool Workshop2::initialize()
{
	//attempt to load our teapot data
	if (!loadTeapot())
		return false;

	if (!terrainshader.loadShaderProgram("teapot"))
	{
		std::cout << terrainshader.getErrorMessage() << std::endl;
		return false;
	}

	//enable depth testing and set to "Less or Equal" mode
	//only pixels that are closer or equally close are shown
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);

	viewpoint = Vector3(0.0f, -400.0f, 0.0f);
	viewtarget = Vector3(0.0f, 0.0f, 0.0f);

	light_position = Vector3(-500.0f, 0.0f, 0.0f);
	light_intensity = Vector3(0.0f, 0.6f, 0.0f);

	time = 0.0;

	return true;
}

void Workshop2::update(int width, int height, double deltatime)
{
    // toggle diffuse by pressing "l"
	if (inputstate.keyspress[SDLK_l]) {
        if(!toonshader)
		    diffuseshader = !diffuseshader;
	}
    //toggle cartoon mode by pressing "a"
	if (inputstate.keyspress[SDLK_a])
        if(!diffuseshader)
		    toonshader = !toonshader;
	//toggle wireframe mode by pressing "w"
	if (inputstate.keyspress[SDLK_w])
		wireFrameMode = !wireFrameMode;
	//turn camera left by pressing left arrow
	if (inputstate.keysdown[SDLK_LEFT]) {
		Vector3 direction = viewtarget - viewpoint;
		Vector3 rotatedDir = direction.rotateZ(turnSpeed / 180.0 * M_PI);
		viewtarget = viewpoint + rotatedDir;
	}
	//turn camera right by pressing right arrow
	if (inputstate.keysdown[SDLK_RIGHT]) {
		Vector3 direction = viewtarget - viewpoint;
		Vector3 rotatedDir = direction.rotateZ(-turnSpeed / 180.0 * M_PI);
		viewtarget = viewpoint + rotatedDir;
	}
	//turn camera up by pressing up arrow
	if (inputstate.keysdown[SDLK_UP]) {
		Vector3 direction = viewtarget - viewpoint;
		Vector3 rotatedDir = direction.rotateX(turnSpeed / 180.0 * M_PI);
		viewtarget = viewpoint + rotatedDir;
	}
	//turn camera down by pressing down arrrow
	if (inputstate.keysdown[SDLK_DOWN]) {
		Vector3 direction = viewtarget - viewpoint;
		Vector3 rotatedDir = direction.rotateX(-turnSpeed / 180.0 * M_PI);
		viewtarget = viewpoint + rotatedDir;
	}
	//move camera forwards by pressing "t"
	if (inputstate.keysdown[SDLK_t]) {
		Vector3 direction = viewtarget - viewpoint;
		Vector3 normalDir = direction.normalize();
		viewpoint += normalDir;
		viewtarget += normalDir;
	}
	//move camera backwards by pressing "g"
	if (inputstate.keysdown[SDLK_g]) {
		Vector3 direction = viewtarget - viewpoint;
		Vector3 normalDir = direction.normalize();
		viewpoint -= normalDir;
		viewtarget -= normalDir;
	}
	//move camera up by pressing "u"
	if (inputstate.keysdown[SDLK_u]) {
		viewpoint += Vector3(0.0f, 0.0f, 1.0f);
		viewtarget += Vector3(0.0f, 0.0f, 1.0f);
	}
	//move camera down by pressing "d"
	if (inputstate.keysdown[SDLK_d]) {
		viewpoint += Vector3(0.0f, 0.0f, -1.0f);
		viewtarget += Vector3(0.0f, 0.0f, -1.0f);
	}

	this->width = width;
	this->height = height;
	this->time += (float)deltatime;
}

void Workshop2::render()
{
	//set the size of the rendering area
	glViewport(0, 0, width, height);
	
	
	//modelview transform matrix
	Matrix4 modelviewmatrix = Matrix4::lookAtMatrix(viewpoint,
	                                                viewtarget,
	                                                Vector3(0.0f, 0.0f, 1.0f));
	//projection transform matrix
	Matrix4 projectionmatrix = Matrix4::perspectiveMatrix((float)M_PI_4, (float)width / height, 1.0f, 10000.0f);

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//use the terrain shader
	glUseProgram(terrainshader.getProgram());
	//set our matrices as "uniform" variables
	glUniformMatrix4fv(glGetUniformLocation(terrainshader.getProgram(), "matmodelview"), 1, GL_TRUE, modelviewmatrix.elements());
	glUniformMatrix4fv(glGetUniformLocation(terrainshader.getProgram(), "matprojection"), 1, GL_TRUE, projectionmatrix.elements());


	glUniform3f(glGetUniformLocation(terrainshader.getProgram(), "camera_position"), viewpoint.x(), viewpoint.y(), viewpoint.z());
	glUniform3f(glGetUniformLocation(terrainshader.getProgram(), "light_intensity"), light_intensity.x(), light_intensity.y(), light_intensity.z());
	glUniform3f(glGetUniformLocation(terrainshader.getProgram(), "light_position"), light_position.x(), light_position.y(), light_position.z());
	glUniform1i(glGetUniformLocation(terrainshader.getProgram(), "toonshader"), toonshader);
	glUniform1i(glGetUniformLocation(terrainshader.getProgram(), "diffuseshader"), diffuseshader);
	

	//set vertex position data
	glBindBuffer(GL_ARRAY_BUFFER, vertexbufferobject_position);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	
	//set vertex normal data
	glBindBuffer(GL_ARRAY_BUFFER, vertexbufferobject_normal);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);


	//draw the data
	if (wireFrameMode)
		glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
	else
		glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
	glDrawArrays(GL_QUADS, 0, teapot_count/2);
	
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}


bool Workshop2::loadTeapot()
{
	//large arrays of position and normal vectors
	//each tile consists of 2 triangles (3 * 2 = 6 vectors)
	std::vector<Vector3> positions(teapot_count/6);
	std::vector<Vector3> normals(teapot_count/6);

	size_t i;
	for (i = 0; i < positions.size(); i++) {
		positions[i] = Vector3(teapot[i*6+0], teapot[i*6+1], teapot[i*6+2] - 100.0f);
	}

	for (i = 0; i < normals.size(); i++){
		normals[i] = Vector3(teapot[i*6+3], teapot[i*6+4], -teapot[i*6+5]);
		normals[i] = normals[i].normalize();
	}
	
	//Use the arrays "positions" and "normals" to fill two vertex buffer objects
	//These are essentially arrays of data to be stored in the GPU memory
	//First create a new vertex buffer object
	glGenBuffers(1, &vertexbufferobject_position);
	//then bind it, so that the next operation is performed on this object
	glBindBuffer(GL_ARRAY_BUFFER, vertexbufferobject_position);
	//send the data to the GPU
	//arguments after GL_ARRAY_BUFFER: size of data in bytes, pointer to start of data, expected method of updating the data
	glBufferData(GL_ARRAY_BUFFER, 0.5 * teapot_count * sizeof(float), &positions[0], GL_STATIC_DRAW);
	
	//repeat of the above, now for the normal array
	glGenBuffers(1, &vertexbufferobject_normal);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbufferobject_normal);
	glBufferData(GL_ARRAY_BUFFER, 0.5 * teapot_count * sizeof(float), &normals[0], GL_STATIC_DRAW);

	return true;
}
