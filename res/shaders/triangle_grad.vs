#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texture_coords;
layout (location = 2) in vec3 normals;
layout (location = 3) in vec3 tangents;
layout (location = 4) in vec3 bitangents;

out vec3 out_Color;
out vec2 out_TextureCoords;

uniform mat4 model, view, projection;
uniform vec3 ball;

void main(void) {

	vec4 worldPosition = model * vec4(position, 1.0),
		 viewPosition  = view * worldPosition;
	gl_Position = projection * viewPosition;

	//out_Color = normals * 0.5 + 0.5;
	out_Color = position + 0.5;
	//out_Color = abs(position - ball);
	out_TextureCoords = out_Color.xy * 2.0;

}
