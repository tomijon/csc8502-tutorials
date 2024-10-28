#version 330 core

in vec3 position;
in vec4 vertexColor;


out Vertex {
	vec4 color;
} OUT;

void main() {
	gl_Position = vec4(position, 1);
	OUT.color = vertexColor;
}