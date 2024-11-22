#version 330 core

uniform sampler2D renderTex;
uniform int isVertical;

in vec2 texCoord;

out vec4 color;

void main(void) {
	color = texture(renderTex, texCoord);
	float mag = length(color.xyz);

	if (mag <= 0.3267) {
		color = vec4(1, 1, 1, 1);
	}
	else if (mag <= 1) {
		color = vec4(0.5, 0.5, 0.5, 1);
	}
	else {
		color = vec4(0, 0, 0, 1);
	}

	color = length(color.xyz) >= 0.75f ? vec4(0, 0, 0, 1) : vec4(1, 1, 1, 1);
}

