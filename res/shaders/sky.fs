#version 330

in vec3 textureCoords;

out vec4 out_Color;

uniform samplerCube texture0;

void main(void){

	// out_Color = mix(texture(texture0, textureCoords), vec4(1.0, 0.0, 0.5, 1.0), 1.0);
	out_Color = texture(texture0, textureCoords);

}
