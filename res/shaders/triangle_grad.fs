#version 330 core

in vec3 unitNormal, unitLightVector, reflectedUnitLightVector, toLightVector, toCameraVector;
in vec2 out_TextureCoords;

layout (location = 0) out vec4 color;

struct Material {
	sampler2D texture0 /* Diffuse texture */, normalMap, specularMap;
	float shineDamper, reflectivity, transparency, fresnelPower;
	int hasDiffuseTexture, hasNormalMap, hasSpecularMap, isTransparent, hasFresnel;
	vec3 diffuse, ambient, specular;
};
uniform Material material;
uniform vec3 lightCol;
uniform float ambientLightIntensity;

void main(void) {

	float lightFactor = max(ambientLightIntensity, dot(unitNormal, unitLightVector));
	vec3 diffuse = lightCol * lightFactor;

	float specularFactor = max(dot(reflectedUnitLightVector, toCameraVector), 0.0);
	float dampedFactor = pow(specularFactor, material.shineDamper);
	vec3 finalSpecular = dampedFactor * material.reflectivity * lightCol;

	color = (texture(material.texture0, out_TextureCoords) * material.hasDiffuseTexture) + (vec4(1.0) * (1.0 - material.hasDiffuseTexture));
	color.rgb *= diffuse;
	color += vec4(finalSpecular, 1.0);

	// color = vec4(out_TextureCoords, 0.0, 1.0);
}
