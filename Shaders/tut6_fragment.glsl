#version 330 core

uniform sampler2D diffuseTex;
uniform int useTexture;

in Vertex {
	vec2 texCoord;
	vec4 hue;
} IN;

out vec4 final;

void main() {
	final = IN.hue;

	if (useTexture > 0) {
		final *= texture(diffuseTex, IN.texCoord);
	}
}