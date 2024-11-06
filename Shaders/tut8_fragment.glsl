#version 330 core

uniform sampler2D diffuseTex;

in Vertex {
	vec2 texCoord;
} IN;

out vec4 final;

void main() {
	final = texture(diffuseTex, IN.texCoord);
}