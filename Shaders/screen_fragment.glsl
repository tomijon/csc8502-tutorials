#version 330 core

uniform sampler2D renderTex;
uniform sampler2D previousTex;

uniform float weight;

in vec2 texCoord;

out vec4 color;

void main() {
	 color = texture(renderTex, texCoord);

	 //color = mix(texture(previousTex, texCoord), texture(renderTex, texCoord), weight);
}