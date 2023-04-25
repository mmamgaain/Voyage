#version 330 core

#ifndef REFRACTIVE_INDEX
#define REFRACTIVE_INDEX 1.0/1.33
#endif

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texture_coords;
layout (location = 2) in vec3 normals;
layout (location = 3) in vec3 tangents;
layout (location = 4) in vec3 bitangents;

out vec3 unitNormal, unitLightVector, reflectedUnitLightVector, toLightVector, toCameraVector;
out vec2 out_TextureCoords;
out float visibility, refractiveFactor;  // New Shit

uniform mat4 model, view, projection;
uniform vec3 lightDir;

void main(void) {

	vec4 worldPosition = model * vec4(position, 1.0),
	     fromCameraPosition  = view * worldPosition;
	gl_Position = projection * fromCameraPosition;

	out_TextureCoords = texture_coords;

	unitNormal = normalize(model * vec4(normals, 0.0)).xyz;
	unitLightVector = normalize(lightDir);
	reflectedUnitLightVector = reflect(unitLightVector, unitNormal);
	toLightVector = -unitLightVector;
	toCameraVector = normalize((inverse(view) * vec4(0.0, 0.0, 0.0, 1.0)).xyz - worldPosition.xyz);
}
