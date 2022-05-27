#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texture_coords;
layout (location = 2) in vec3 normals;
layout (location = 3) in vec3 tangents;
layout (location = 4) in vec3 bitangents;

out vec3 out_Color;

uniform mat4 model, view, projection;

void main(void) {

	//gl_Position = projection * view * model * vec4(position, 1.0);
	gl_Position = vec4(position, 1.0);

	//out_Color = normals * 0.5 + 0.5;
	out_Color = vec3(0.0, 1.0, 0.0);

}
