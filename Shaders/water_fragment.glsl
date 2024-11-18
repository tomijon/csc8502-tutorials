#version 330 core

uniform vec3 lightPosition;
uniform vec3 cameraPosition;

uniform vec4 ambientColor;
uniform float ambientPower;

uniform vec4 shadowColor;
uniform float diffusePower;

uniform vec4 specularColor;
uniform float specularPower;
uniform float specularWeight;

in vec3 fragPosition;
in vec3 normal;
in vec3 tangent;
in vec3 binormal;
in vec2 texCoord;
in float height;

out vec4 final;


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


void main() {
	vec3 incident = normalize(lightPosition - fragPosition);
	vec3 view = normalize(cameraPosition - fragPosition);
	vec3 halfway = normalize(incident + view);

	final = vec4(0.11, 0.16, 0.65, 1);
	
	final = mix(final, ambientColor, ambientPower); 
	final = diffuse(final, incident, normal);
	final = specular(final, incident, halfway, normal);
	final.a = 0.85;
}
