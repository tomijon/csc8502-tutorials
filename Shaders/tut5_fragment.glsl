#version 330 core

uniform sampler2D diffuseTex;

in Vertex {
	vec2 texCoord;
	vec4 hue;
} IN;

out vec4 final;

void main() {
	vec4 value = (0.5 * texture(diffuseTex, IN.texCoord)) + (0.5 * IN.hue);

	if (value == 0.0) discard;
	final = value;
}