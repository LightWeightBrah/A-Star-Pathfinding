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

in vec3 FragPos;
in vec3 Normal;

out vec4 FragColor;

uniform vec3 objectColor;
uniform vec3 lightSourceColor;
uniform	vec3 lightSourcePosition;
uniform vec3 viewerPosition;

void main()
{
	//ambient light
	float ambientStrength = 0.1f;
	vec3 ambient = ambientStrength * lightSourceColor;

	//diffuse light
	vec3 norm = normalize(Normal);
	vec3 lightDirection = normalize(lightSourcePosition - FragPos);
	float dotProductAngle = max(dot(norm, lightDirection), 0.0f);
	vec3 diffuse = dotProductAngle * lightSourceColor;

	//specular light
	float specularStrength = 0.5f;
	vec3 viewerDirection = normalize(viewerPosition - FragPos);
	vec3 reflectDirection = reflect(-lightDirection, norm);
	const int shininess = 32;
	float spec = pow(max(dot(viewerDirection, reflectDirection), 0.0f), shininess);
	vec3 specular = specularStrength * spec * lightSourceColor;

	vec3 result = (ambient + diffuse + specular) * objectColor;

	FragColor = vec4(result, 1.0f);
}