#version 330

in vec2 out_textureCoords;

layout (location = 0) out vec4 color;

uniform float time;
uniform sampler2D texture0;

void main(void) {

	color = texture(texture0, out_textureCoords);

}
