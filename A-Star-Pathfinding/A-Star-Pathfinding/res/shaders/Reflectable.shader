#shader vertex
#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 FragPos;
out vec3 Normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0);
	FragPos = vec3(model * vec4(aPos, 1.0f));
	Normal = mat3(transpose(inverse(model))) * aNormal;
}

#shader fragment
#version 330 core

struct Material
{
	vec3 ambientColor;
	vec3 diffuseColor;
	vec3 specularColor;
	float shininess;
};

struct LightSource
{
	vec3 position;

	vec3 ambientStrength;
	vec3 diffuseStrength;
	vec3 specularStrength;

};

in vec3 FragPos;
in vec3 Normal;

out vec4 FragColor;

uniform Material material;
uniform LightSource lightSource;

uniform vec3 viewerPosition;

void main()
{
	//ambient light
	vec3 ambient = lightSource.ambientStrength * material.ambientColor; 

	//diffuse light
	vec3 norm = normalize(Normal);
	vec3 lightDirection = normalize(lightSource.position - FragPos);
	float dotProductAngle = max(dot(norm, lightDirection), 0.0f);
	vec3 diffuse = lightSource.diffuseStrength * (dotProductAngle * material.diffuseColor);

	//specular light
	vec3 viewerDirection = normalize(viewerPosition - FragPos);
	vec3 reflectDirection = reflect(-lightDirection, norm);
	float spec = pow(max(dot(viewerDirection, reflectDirection), 0.0f), material.shininess);
	vec3 specular = lightSource.specularStrength * (spec * material.specularColor); 

	vec3 result = ambient + diffuse + specular;

	FragColor = vec4(result, 1.0f);
}