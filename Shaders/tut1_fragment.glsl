#version 330 core

in Vertex {
	vec4 color;
} IN;

out vec4 final;

void main() {
	final = IN.color;
}