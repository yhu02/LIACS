#version 130
struct Material{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};
uniform Material material;
 
// Output color
out vec4 fragcolor;

uniform vec3 light_position;
uniform vec3 light_intensity;
uniform vec3 camera_position;

// Variables from vertex to fragment shader
in vec3 _normal; //normal vector
in vec4 _position; //position vector


void main()
{
    Material mat = material;
    // ambient
    vec3 ambient = clamp(material.ambient * light_intensity, 0.0, 1.0);
  	
    // diffuse 
    vec3 norm = normalize(_normal);
    vec3 light_dir = normalize(light_position - _position.xyz); 
    float NdotL = dot(norm, light_dir);
    vec3 diffuse = clamp(material.diffuse * light_intensity * NdotL, 0.0, 1.0);
    
    // specular fill in your own formula        
    vec3 k_s = mat.specular;
    vec3 I_light = light_intensity;
    vec3 V = normalize(camera_position - _position.xyz);
    vec3 N = normalize(_normal);
    vec3 H = ((light_dir + V) / length(light_dir + V));
    float n_shiny = mat.shininess;

    vec3 specular = k_s * I_light * pow(dot(N, H), n_shiny);

    vec3 result = ambient + diffuse + specular;
    fragcolor = vec4(result, 1.0);
}
