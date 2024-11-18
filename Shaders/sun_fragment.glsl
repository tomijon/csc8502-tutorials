#version 330 core

uniform vec3 lightPosition;
uniform vec3 cameraPosition;

uniform sampler2D sunTex;

in vec3 fragPosition;
in vec3 normal;
in vec3 tangent;
in vec3 binormal;
in vec2 texCoord;
in float height;

out vec4 final;


vec4 textureColor() {
	return texture(sunTex, texCoord);
}


void main() {
	final = textureColor();
}
