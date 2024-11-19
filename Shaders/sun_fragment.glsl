#version 330 core

uniform vec3 lightPosition;
uniform vec3 cameraPosition;
uniform float elapsedTime;

uniform sampler2D sunTex;
uniform sampler2D sunBump;

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

vec4 brightness(vec4 color) {
	vec4 brightnessColor = vec4(1, 1, 1, 1) - texture(sunBump, texCoord);
	brightnessColor.a = 1;
	vec4 scaled = brightnessColor * 4;
	return color * scaled;
}


void main() {
	final = textureColor();
	final = brightness(final);
}
