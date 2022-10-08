#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texture_coords;
layout (location = 2) in vec3 normals;
layout (location = 3) in vec3 tangents;
layout (location = 4) in vec3 bitangents;

out vec3 unitNormal, unitLightVector, reflectedUnitLightVector, toLightVector, toCameraVector;
out vec2 out_TextureCoords;

uniform mat4 model, view, projection;
uniform vec3 lightDir;

void main(void) {

	vec4 worldPosition = model * vec4(position, 1.0),
	     viewPosition  = view * worldPosition;
	gl_Position = projection * viewPosition;

	unitNormal = normalize(model * vec4(normals, 0.0)).xyz;
	unitLightVector = normalize(lightDir);
	reflectedUnitLightVector = reflect(unitLightVector, unitNormal);
	toLightVector = -unitLightVector;
	toCameraVector = normalize((inverse(view) * vec4(0.0, 0.0, 0.0, 1.0)).xyz - worldPosition.xyz);

	out_TextureCoords = position.xy + 0.5 * 2.0;
}
