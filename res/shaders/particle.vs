#version 330

layout (location = 0) in vec2 position;
layout (location = 1) in mat4 modelView;
layout (location = 5) in vec4 texOffsets;
layout (location = 6) in float blend;

out float out_blend;
out vec2 textureCoordsCurr, textureCoordsNext;

uniform mat4 project;
uniform vec2 texMapDimensions;

void main(void) {

	gl_Position = project * modelView * vec4(position, 0.0, 1.0);

	vec2 textureCoords = (position * 0.5 + 0.5) / texMapDimensions;

	textureCoordsCurr = textureCoords + texOffsets.xy;
	textureCoordsNext = textureCoords + texOffsets.zw;
	textureCoordsCurr.y = 1.0 - textureCoordsCurr.y;
	textureCoordsNext.y = 1.0 - textureCoordsNext.y;

	out_blend = blend;

}
