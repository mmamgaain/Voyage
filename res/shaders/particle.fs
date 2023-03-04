#version 330

in vec2 textureCoordsCurr, textureCoordsNext;

layout (location = 0) out vec4 out_Color;

uniform sampler2D texture0;
uniform float blend;

void main(void) {

	vec4 colorCurr = texture(texture0,textureCoordsCurr),
		 colorNext = texture(texture0, textureCoordsNext);

	out_Color = mix(colorCurr, colorNext, blend);

}
