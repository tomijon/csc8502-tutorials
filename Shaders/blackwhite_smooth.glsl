
#version 330 core

uniform sampler2D renderTex;
uniform int isVertical;

in vec2 texCoord;

out vec4 color;

void main(void) {
	vec2 delta = vec2(0, 0);
	delta += dFdy(texCoord) + dFdx(texCoord);

	int totalWhite = 0;
	int totalBlack = 0;

	for (int x = -1; x < 2; x++) {
		for (int y = -1; y < 2; y++) {
			vec2 offset = delta;
			offset.x = delta.x * x;
			offset.y = delta.y * y;
			vec4 current = texture(renderTex, texCoord + offset);

			if (length(color.xyz) >= 0.75f) totalBlack++;
			else totalWhite++;

		}
	}
	color = totalWhite + 5 > totalBlack ? vec4(0, 0, 0, 1) : vec4(1, 1, 1, 1);
}

