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
uniform float specularWeight;

uniform vec4 fogColor;
uniform float fogDensity;
uniform float fogStrength;

uniform float snowHeightStart;
uniform float mountainHeightStart;
uniform float grassHeightStart;


in vec3 fragPosition;
in vec3 normal;
in vec3 tangent;
in vec3 binormal;
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
vec4 diffuse(vec4 color, vec3 incident, vec3 bumpNormal) {
	float diffuseFactor = clamp(dot(incident, bumpNormal), 0, 1);
	return mix(color, shadowColor, (1 - diffuseFactor) * diffusePower);
}


// Applies specular highlights.
vec4 specular(vec4 color, vec3 incident, vec3 halfway, vec3 bumpNormal) {
	float specularFactor = pow(clamp(dot(bumpNormal, halfway), 0, 1), specularPower);
	return mix(color, specularColor, specularFactor * specularWeight);
}


// Convert to bump normal.
vec3 getWeightedNormal() {
	vec3 snowNormal = texture(snowBump, texCoord).rgb;
	vec3 mountainNormal = texture(mountainBump, texCoord).rgb;
	vec3 grassNormal = texture(grassBump, texCoord).rgb;
	vec3 sandNormal = texture(sandBump, texCoord).rgb;

	vec3 weighted;

	if (height >= snowHeightStart) {
		weighted = mix(mountainNormal, snowNormal, (height - snowHeightStart) / (1 - snowHeightStart));
	}
	else if (between(height, mountainHeightStart, snowHeightStart)) {
		weighted = mix(grassNormal, mountainNormal, (height - mountainHeightStart) / (snowHeightStart - mountainHeightStart));
	}
	else if (between(height, grassHeightStart, mountainHeightStart)) {
		weighted = mix(sandNormal, grassNormal, (height - grassHeightStart) / (mountainHeightStart - grassHeightStart));
	}
	else weighted = sandNormal;
	return normalize(weighted);
}



// Fog calculations.
vec4 applyFog(vec4 color) {
	float dist = length(fragPosition - cameraPosition);
	float fogFactor = clamp(exp(fogDensity * dist), 0, 1);
	return mix(color, fogColor, fogFactor * fogStrength);
}


void main() {
	final = textureColor();

	vec3 incident = normalize(lightPosition - fragPosition);
	vec3 view = normalize(cameraPosition - fragPosition);
	vec3 halfway = normalize(incident + view);

	mat3 TBN = mat3(normalize(tangent), normalize(binormal), normalize(normal));
	vec3 bumpNormal = getWeightedNormal();
	bumpNormal = normalize(TBN * normalize(bumpNormal * 2.0 - 1.0));


	final = mix(final, ambientColor, ambientPower);
	final = diffuse(final, incident, bumpNormal);
	final = specular(final, incident, halfway, bumpNormal);

//	final = applyFog(final);

}