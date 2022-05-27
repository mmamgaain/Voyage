#version 330

in vec2 out_textureCoords;

out (location=0) vec4 color;

uniform float time;
uniform sampler2D texture0;

void main(void) {

	//color = texture(texture0, out_textureCoords);
	color = vec4(1.0, 0.4, 0.2, 1.0);

}
