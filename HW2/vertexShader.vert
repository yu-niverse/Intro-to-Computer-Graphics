#version 330 core
// TO DO:
// Implement vertex shader
// note: remember to set gl_Position

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;
uniform mat4 S;

out vec2 texCoord;
out vec4 worldPos;
out vec3 normal;

void main()
{
	gl_Position = P * V * M * S * vec4(aPos, 1.0);
	texCoord = aTexCoord;
	worldPos = M * vec4(aPos, 1.0);
	mat4 normal_transform = transpose(inverse(M));
	normal = normalize((normal_transform * vec4(aNormal, 0.0)).xyz);
}