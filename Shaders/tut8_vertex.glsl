#version 330 core

layout(location=0) in vec3 positionIn;
layout(location=2) in vec2 texCoordIn;
layout(location=3) in vec3 normalIn;

uniform mat4 projMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

out vec3 fragPosition;
out vec3 normal;
out vec2 texCoord;
out float height;

void main() {
	fragPosition = vec3(modelMatrix * vec4(positionIn, 1.0));
	height = (length(positionIn) - (length(normalize(positionIn))) * 128) / 8;
	normal = normalIn;

	mat4 mvp = projMatrix * viewMatrix * modelMatrix;
	gl_Position = mvp * vec4(positionIn, 1);
	texCoord = texCoordIn;
}