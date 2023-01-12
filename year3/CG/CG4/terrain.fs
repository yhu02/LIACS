#version 130
struct Material{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};
uniform bool watershader;

uniform Material mountain_mat;
uniform Material water_mat;
 
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
    //material selection
    Material mat;
    mat = mountain_mat;

    if(watershader && _position.z <= 1){
        mat = water_mat;
    }
    //ambient
    vec3 ambient = clamp(mat.ambient * light_intensity, 0.0, 1.0);

    //diffuse
    vec3 norm = normalize(_normal);
    vec3 light_dir = normalize(light_position - _position.xyz);
    float NdotL = dot(norm, light_dir);
    vec3 diffuse = clamp(mat.diffuse * NdotL * light_intensity, 0.0, 1.0);

    //specular
    vec3 specular = vec3(0.0, 0.0, 0.0);
    
    if(watershader && _position.z <= 1){
        vec3 k_s = mat.specular;
        vec3 I_light = light_intensity;
        vec3 V = normalize(camera_position - _position.xyz);
        vec3 N = normalize(_normal);
        vec3 H = ((light_dir + V) / length(light_dir + V));
        float n_shiny = mat.shininess;

        specular = k_s * I_light * pow(dot(N, H), n_shiny);
    }

    vec3 light = ambient + diffuse + specular;
    fragcolor = vec4(light, 1.0);	
  
}
