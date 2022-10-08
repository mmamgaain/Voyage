#version 330 core

in vec3 unitNormal, unitLightVector, reflectedUnitLightVector, toLightVector, toCameraVector;
in vec2 out_TextureCoords;

layout (location = 0) out vec4 color;

// struct Material {
// 	sampler2D texture0 /* Diffuse texture */, normalMap, specularMap;
// 	float shineDamper, reflectivity, transparency, fresnelPower;
// 	bool hasDiffuseTexture, hasNormalMap, hasSpecularMap, isTransparent, hasFresnel;
// 	vec3 diffuse, ambient, specular;
// };

uniform float reflectivity, shineDamper;
uniform sampler2D texture0;
uniform vec3 lightCol;

void main(void) {

	float lightFactor = max(0.2, dot(unitNormal, unitLightVector));
	vec3 diffuse = lightCol * lightFactor;

	float specularFactor = max(dot(reflectedUnitLightVector, toCameraVector), 0.0);
	float dampedFactor = pow(specularFactor, shineDamper);
	vec3 finalSpecular = dampedFactor * reflectivity * lightCol;

	color = texture(texture0, out_TextureCoords);
	color.rgb *= diffuse;
	color += vec4(finalSpecular, 1.0);
}
