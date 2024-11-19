#version 330 core

layout(location = 0) in vec3 positionIn;
layout(location = 2) in vec2 texCoordIn;
layout(location = 3) in vec3 normalIn;
layout(location = 4) in vec4 tangentIn;

uniform float elapsedTime;

uniform mat4 projMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

out vec3 fragPosition;
out vec3 normal;
out vec3 tangent;
out vec3 binormal;

out vec2 texCoord;
out float height;

void main() {
	fragPosition = vec3(modelMatrix * vec4(positionIn, 1.0));
	height = (length(positionIn) - (length(normalize(positionIn))) * 128) / 4;

	mat3 normalMat = transpose(inverse(mat3(modelMatrix)));

	vec3 wNormal = normalize(normalMat * normalize(normalIn));
	vec3 wTangent = normalize(normalMat * normalize(tangentIn.xyz));

	normal = wNormal;
	tangent = wTangent;
	binormal = cross(wTangent, wNormal) * tangentIn.w;

	vec3 altered = positionIn + (normalize(positionIn) * sin(elapsedTime) * 0.5);

	mat4 mvp = projMatrix * viewMatrix * modelMatrix;
	gl_Position = mvp * vec4(altered, 1);
	texCoord = texCoordIn;
}
