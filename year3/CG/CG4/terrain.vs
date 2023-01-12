#version 130
 
uniform mat4 matmodelview;
uniform mat4 matprojection;

in vec4 in_position;
in vec3 in_normal;
in float time;

// End of header part

// Variables from vertex to fragment shader
out vec3 _normal;
out vec4 _position;


void main()
{
	// Get a copy of the normal vector
    _normal = 2.0*in_normal;
    _position = in_position;
	
    if(_position.z <= 1) _position.z = 1;
	// Transform the vertex position to screencoordinates
	gl_Position = matprojection * (matmodelview * _position);
	
}
