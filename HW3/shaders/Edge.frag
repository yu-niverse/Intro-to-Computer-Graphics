#version 330 core

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

in vec2 texCoord;
in vec4 worldPos;
in vec3 normal;

uniform vec3 cameraPos;
uniform Material material;
uniform Light light;

out vec4 fragColor;

void main()
{
	vec3 L = normalize(light.position - worldPos.xyz),
		 V = normalize(cameraPos - worldPos.xyz),
		 R = normalize(reflect(-L, normal)),
		 N = normalize(normal);
	
	if (dot(V, N) < 0.15 && dot(V, N) > -0.15) 
        fragColor = vec4(0.0f, 0.0f, 1.0f, 1.0f);
    else if (dot(V, N) < 0.18 && dot(V, N) > -0.18) 
        fragColor = vec4(0.0f, 0.0f, 0.8f, 1.0f);
    else if (dot(V, N) < 0.2 && dot(V, N) > -0.2) 
        fragColor = vec4(0.0f, 0.0f, 0.6f, 1.0f);
    else if (dot(V, N) < 0.23 && dot(V, N) > -0.23) 
        fragColor = vec4(0.0f, 0.0f, 0.4f, 1.0f);
    else fragColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);
}