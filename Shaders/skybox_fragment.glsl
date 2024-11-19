#version 330 core

uniform samplerCube cubeTex;
uniform vec3 cameraPosition;

in Vertex {
	vec3 viewDir;
} IN;

out vec4 fragColour;

void main(void) {
	fragColour = texture(cubeTex, normalize(IN.viewDir));

	vec4 fadeColor = vec4(0.51, 0.91, 0.97, 1);
	float power = clamp(1 - (length(cameraPosition) / 300), 0, 0.8);
	fragColour = mix(fragColour, fadeColor, power);
}
