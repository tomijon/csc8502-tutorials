#version 330 core

uniform sampler2D diffuseTex;
uniform vec3 lightDirection;
uniform vec3 cameraPosition;

vec4 SNOW = vec4(1, 1, 1, 1);
vec4 MOUNTAIN = vec4(0.5, 0.5, 0.5, 1);
vec4 GRASS = vec4(0.75, 0.84, 0.5, 1);
vec4 WATER = vec4(0.26, 0.52, 0.67, 1);

vec4 shadowColor = vec4(0, 0, 0, 1);
vec4 fogColor = vec4(1, 1, 1, 1);
float fogStart = 2000;
float fogEnd = 30000;
float fogDensity = 0.0001;
float diffusePower = 0.5;

in vec3 fragPosition;
in vec3 normal;
in vec2 texCoord;
in float height;

out vec4 final;

void main() {
	vec4 texture_color = texture(diffuseTex, texCoord);
	vec4 height_color;

	if (height > 0.7) height_color = SNOW;
	else if (height > 0.5) height_color = MOUNTAIN;
	else if (height > 0.2) height_color = GRASS;
	else height_color = WATER;

	float diffuse = clamp(dot(lightDirection, normal), 0, 1);
	float dist = length(cameraPosition - fragPosition);
//	float fogFactor = clamp((dist - fogStart) / (fogEnd - fogStart), 0, 1);
	float fogFactor = exp(-fogDensity * dist);


	final = mix(texture_color, height_color, 0.5);
	final = mix(final, shadowColor, diffuse * diffusePower);
	final = mix(final, fogColor, 1 - fogFactor);
}