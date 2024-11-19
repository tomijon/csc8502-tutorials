#version 330 core

uniform sampler2D waterBump;
uniform samplerCube cubeTex;

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


// Applies skybox reflections.
vec4 skybox(vec4 color, vec3 view, vec3 normal) {
	vec3 reflectVec = reflect(-view, normal);
	vec4 reflectColor = texture(cubeTex, reflectVec);
	vec4 fadeColor = vec4(0.51, 0.91, 0.97, 1);
	float power = clamp(1 - (length(cameraPosition) / 500), 0, 0.8);
	reflectColor = mix(reflectColor, fadeColor, power);
	return mix(color, reflectColor, power); 
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


void main() {
	mat3 TBN = mat3(normalize(tangent), normalize(binormal), normalize(normal));
	vec3 bumpNormal = texture(waterBump, texCoord).rgb;
	bumpNormal = normalize(TBN * normalize(bumpNormal * 2.0 - 1.0));

	vec3 incident = normalize(lightPosition - fragPosition);
	vec3 view = normalize(cameraPosition - fragPosition);
	vec3 halfway = normalize(incident + view);

	final = vec4(0.11, 0.16, 0.25, 1);
	
	final = mix(final, ambientColor, ambientPower); 
	final = skybox(final, view, bumpNormal);
	final = diffuse(final, incident, bumpNormal);
	final = specular(final, incident, halfway, bumpNormal);
	final.a = 0.8;
}
