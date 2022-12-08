#version 330 core
// TO DO:
// Implement fragment shader

in vec2 texCoord;
in vec4 worldPos;
in vec3 normal;

uniform sampler2D ourTexture;
uniform int darken;
uniform int bonus;

out vec4 FragColor;

void main()
{
	FragColor = texture(ourTexture, texCoord);
	if (darken == 1) {
		if (FragColor.r < 0.45 && FragColor.g < 0.45 && FragColor.b < 0.45) {
			FragColor.rgb = vec3(0.25, 0.1, 0.1);
		}
	}

	if (bonus == 1) {
		if (FragColor.r < 0.5 && FragColor.g < 0.5 && FragColor.b < 0.5)
			FragColor.rgb = vec3(1, 0.83, 0.83);
	}
	else if (bonus == 3) {
		if (FragColor.r < 0.5 && FragColor.g < 0.5 && FragColor.b < 0.5)
			FragColor.rgb = vec3(1, 0.96, 0.83);
	}
	else if (bonus == 5) {
		if (FragColor.r < 0.5 && FragColor.g < 0.5 && FragColor.b < 0.5)
			FragColor.rgb = vec3(0.83, 1, 0.96);
	}
	else if (bonus == 7) {
		if (FragColor.r < 0.5 && FragColor.g < 0.5 && FragColor.b < 0.5)
			FragColor.rgb = vec3(0.83, 0.83, 1);
	}
}