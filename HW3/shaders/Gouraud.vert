#version 330 core

// TODO:
// Implement Gouraud shading

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float gloss;
};

struct Light { 
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
	vec3 position;  
};

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;
uniform Material material;
uniform Light light;
uniform vec3 cameraPos;

out vec2 texCoord;
out vec4 ambient;
out vec4 diffuse;
out vec4 specular;

void main()
{
	gl_Position = P * V * M * vec4(aPos, 1.0);
	texCoord = aTexCoord;
	vec4 worldPos = M * vec4(aPos, 1.0);
	mat4 normal_transform = transpose(inverse(M));
	vec3 normal = normalize((normal_transform * vec4(aNormal, 0.0)).xyz);

	vec3 L = normalize(light.position - worldPos.xyz),
		 V = normalize(cameraPos - worldPos.xyz),
		 R = normalize(reflect(-L, normal)),
		 N = normalize(normal);

	vec4 Ka = vec4(material.ambient, 1.0f),
		 Kd = vec4(material.diffuse, 1.0f),
		 Ks = vec4(material.specular, 1.0f),
		 La = vec4(light.ambient, 1.0f),
		 Ld = vec4(light.diffuse, 1.0f),
		 Ls = vec4(light.specular, 1.0f);

	ambient = La * Ka;
	diffuse = Ld * Kd * max(dot(L, N), 0.0);
	specular = Ls * Ks * pow(max(dot(V, R), 0.0), material.gloss);	
}
