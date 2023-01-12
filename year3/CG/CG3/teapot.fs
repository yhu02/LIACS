#version 130
 
// Output color
out vec4 fragcolor;

uniform bool toonshader;
uniform bool diffuseshader; 
uniform vec3 light_position;
uniform vec3 light_intensity;

// Variables from vertex to fragment shader
in vec3 _normal; //normal vector
in vec4 _position; //position vector


void main()
{
    //simplified lighting model 
    vec3 light = light_intensity;
    float intensity = _normal.x*0.5 + (1 - _normal.z)*0.5;
    float kd = 0.5;
    
    if(toonshader) intensity = int(_normal.x * 5)/5.0 ; // ambient lighting
    if(diffuseshader) intensity = kd * intensity * dot(_normal, reflect(light_intensity, normalize(_normal)));
    light = clamp(light * intensity, 0.0, 1.0);
    fragcolor = vec4(light, 1.0);
}
