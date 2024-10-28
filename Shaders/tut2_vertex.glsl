#version 330 core

in vec3 position;
in vec4 vertexColor;

uniform mat4 projMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

out Vertex {
	vec4 color;
} OUT;

void main() {
	mat4 mvp = projMatrix * viewMatrix * modelMatrix;
	gl_Position = mvp * vec4(position, 1);
	OUT.color = vertexColor;
}