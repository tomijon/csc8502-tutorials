#version 330 core

uniform sampler2D renderTex;
uniform int isVertical;

in vec2 texCoord;

out vec4 color;

 const float scaleFactors[7] =
 float[](0.006, 0.061, 0.242, 0.383, 0.242, 0.061, 0.006);

 void main(void) {
	 color = vec4(0, 0, 0, 1);
	 
	 vec2 delta = vec2(0, 0);
 
	 if (isVertical == 1) {
		 delta = dFdy(texCoord);
	 }

	 else {
		 delta = dFdx(texCoord);
	 }

	 for (int i = 0; i < 7; i++) {
		 vec2 offset = delta * (i - 3);
		 vec4 tmp = texture2D(renderTex, texCoord + offset);
		 color += tmp * scaleFactors[i];
	 }
 }