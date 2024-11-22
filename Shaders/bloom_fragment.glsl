#version 330 core

uniform sampler2D renderTex;

in vec2 texCoord;

out vec4 color;


 void main(void) {
	 color = texture(renderTex, texCoord);
	 color *= 1.05;
 }