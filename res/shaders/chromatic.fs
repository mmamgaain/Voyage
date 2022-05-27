#version 330

#define err vec2(0.0, 0.005)
#define screen vec2(1600, 900)

in vec2 textureCoords;

layout (location = 0) out vec4 out_Color;

uniform sampler2D texture0;

void main(void) {
	
	vec2 ndc = (gl_FragCoord.xy / screen) * 2.0 - 1.0;
	
	out_Color = vec4(texture(texture0, textureCoords - (ndc * err.yx)).r,
					 texture(texture0, textureCoords - (ndc * err)).g,
					 texture(texture0, textureCoords - (ndc * err.yy)).b,
					 1.0);
	
}