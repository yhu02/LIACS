#version 130

// Output color
out vec4 fragcolor;

// Variables from vertex to fragment shader
in vec3 _normal; //normal vector
in vec4 _position; //position vector

void main()
{
	//simplified lighting model
    if(_position.z > 69)
    {
        fragcolor = _normal.z * vec4(0.3, 0.8, 0.2, 1.0);
    }else{
        fragcolor = _normal.z * vec4(0.1, 0.4, 0.1, 1.0);
    }
}
