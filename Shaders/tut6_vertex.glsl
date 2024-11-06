#version 330 core

in vec3 position;
in vec2 texCoord;

uniform mat4 projMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;
uniform vec4 nodeColour;

out Vertex {
	vec2 texCoord;
	vec4 hue;
} OUT;

void main() {
	mat4 mvp = projMatrix * viewMatrix * modelMatrix;
	gl_Position = mvp * vec4(position, 1);
	OUT.texCoord = texCoord;
	OUT.hue = nodeColour;
}