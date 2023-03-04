#version 330

layout (location = 0) in vec2 position;

out vec2 textureCoordsCurr, textureCoordsNext;

uniform mat4 project, modelView;
uniform vec2 texOffsetCurr, texOffsetNext, texMapDimensions;

void main(void) {

	gl_Position = project * modelView * vec4(position, 0.0, 1.0);

	vec2 textureCoords = (position * 0.5 + 0.5) / texMapDimensions;
	textureCoordsCurr = textureCoords + texOffsetCurr;
	textureCoordsNext = textureCoords + texOffsetNext;

}
