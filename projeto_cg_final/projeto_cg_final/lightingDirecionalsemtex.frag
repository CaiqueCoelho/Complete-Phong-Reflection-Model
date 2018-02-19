#version 330 core
struct Material{
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

struct Light{
	vec3 direction;
	//vec3 position;
	
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	//float constant;
	//float linear;
	//float specular;
};


out vec4 color;

in vec3 FragPos;
in vec3 Normal;


uniform vec3 viewPos;
uniform Material material;
uniform Light light;
uniform vec3 objectColor;
uniform vec3 lightColor;

void main()
{
	//luz ambiente
	vec3 ambient = light.ambient;

	//luz difusa
	vec3 norm = normalize(Normal);
	//vec3 lightDir = normalize(light.position - FragPos);
	//lightDir para luz direcional
	vec3 lightDir = normalize(-light.direction);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = light.diffuse * diff;

	//luz specular
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.specular * spec;

	vec3 result = ambient + diffuse + specular;
	color = vec4(result * objectColor, 1.0f);
}