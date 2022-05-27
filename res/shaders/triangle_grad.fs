#version 330 core

in vec3 out_Color;

layout (location = 0) out vec4 color;

uniform sampler2D texture0;

void main(void) {

	//color = texture(texture0, out_Color);
	color = vec4(out_Color, 1.0);
	//color = vec4(1.0, 1.0, 1.0, 1.0);

}
