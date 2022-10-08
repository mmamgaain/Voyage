#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 textureCoords;
layout (location = 2) in vec3 normals;

uniform mat4 transform, view, project;

void main(void){

	vec4 worldPosition = transform * vec4(position, 1.0),
		 fromCameraPosition = view * worldPosition;
	gl_Position = project * fromCameraPosition;	
}
