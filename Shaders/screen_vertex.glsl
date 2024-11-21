#version 330 core

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;

layout(location = 0) in vec3 position;
layout(location = 2) in vec2 texcoord;

out vec2 texCoord;

void main(void) {
	texCoord = texcoord;
	gl_Position = vec4(position, 1.0);
}
