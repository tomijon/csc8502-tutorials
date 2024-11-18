#version 330 core

uniform sampler2D snowTex;
uniform sampler2D mountainTex;
uniform sampler2D grassTex;
uniform sampler2D sandTex;

uniform sampler2D snowBump;
uniform sampler2D mountainBump;
uniform sampler2D grassBump;
uniform sampler2D sandBump;

uniform vec3 lightPosition;
uniform vec3 cameraPosition;

uniform vec4 ambientColor;
uniform float ambientPower;

uniform vec4 shadowColor;
uniform float diffusePower;

uniform vec4 specularColor;
uniform float specularPower;

uniform vec4 fogColor;
uniform float fogDensity;
uniform float fogStrength;

uniform float snowHeightStart;
uniform float mountainHeightStart;
uniform float grassHeightStart;


in vec3 fragPosition;
in vec3 normal;
in vec2 texCoord;
in float height;

out vec4 final;

#define PI 3.141592

bool between(float value, float low, float high) {
	return value >= low && value <= high;
}


// Fetches and mixes the colours from the textures based on the input height.
vec4 textureColor() {
	vec4 color;

	vec4 snowColor = texture(snowTex, texCoord);
	vec4 mountainColor = texture(mountainTex, texCoord);
	vec4 grassColor = texture(grassTex, texCoord);
	vec4 sandColor = texture(sandTex, texCoord);

	if (height >= snowHeightStart) {
		return mix(mountainColor, snowColor, (height - snowHeightStart) / (1 - snowHeightStart));
	}
	else if (between(height, mountainHeightStart, snowHeightStart)) {
		return mix(grassColor, mountainColor, (height - mountainHeightStart) / (snowHeightStart - mountainHeightStart));
	}
	else if (between(height, grassHeightStart, mountainHeightStart)) {
		return mix(sandColor, grassColor, (height - grassHeightStart) / (mountainHeightStart - grassHeightStart));
	}
	else return sandColor;
}


// Applies diffuse to the color.
vec4 diffuse(vec4 color, vec3 incident) {
	float diffuseFactor = clamp(dot(incident, normal), 0, 1);
	return mix(color, shadowColor, diffuseFactor * diffusePower);
}


// Applies specular highlights.
vec4 specular(vec4 color, vec3 incident) {
    vec3 cameraDirection = normalize(cameraPosition - fragPosition);
	float specularFactor = clamp(dot(incident, cameraDirection), 0, 1);
	return mix(color, specularColor, specularFactor * specularPower);
}


// Fog calculations.
vec4 applyFog(vec4 color) {
	float dist = length(cameraPosition - fragPosition);
	float fogFactor = clamp(exp(fogDensity * dist), 0, 1);
	return mix(color, fogColor, fogFactor * fogStrength);
}


void main() {
	final = textureColor();

//	vec3 incident = normalize(lightPosition - fragPosition);
//	final = mix(final, ambientColor, ambientPower);
//	final = diffuse(final, incident);
//	final = specular(final, incident);
//
//	final = applyFog(final);

}