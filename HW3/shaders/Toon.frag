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

uniform sampler2D catTexture;
uniform Material material;
uniform Light light;
uniform vec3 cameraPos;

out vec4 fragColor;

void main()
{
    vec4 objectColor = texture2D(catTexture, texCoord);

	vec4 Ks = vec4(material.specular, 1.0f),
         Kd = vec4(material.diffuse, 1.0f),
		 Ls = vec4(light.specular, 1.0f);

	vec3 L = normalize(light.position - worldPos.xyz),
		 V = normalize(cameraPos - worldPos.xyz),
		 R = normalize(reflect(-L, normal)),
		 N = normalize(normal);

	vec4 specular = Ls * Ks * pow(max(dot(V, R), 0.0), material.gloss);

    float intensity = 0.6,
          low_intensity = 0.3,
          high_intensity = 1.0;

    if (dot(L, N) < 0) 
        intensity = low_intensity;
    else if (specular.x > 0.01 || specular.y > 0.01 || specular.z > 0.01)
        intensity = high_intensity;

    fragColor = Kd * objectColor * intensity;
}