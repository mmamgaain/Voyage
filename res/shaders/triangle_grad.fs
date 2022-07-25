#version 330 core

in vec3 out_Color;
in vec2 out_TextureCoords;

layout (location = 0) out vec4 color;

struct Material {
	sampler2D texture0 /* Diffuse texture */, normalMap, specularMap;
	float shineDamper, reflectivity, transparency, fresnelPower;
	bool hasDiffuseTexture, hasNormalMap, hasSpecularMap, isTransparent, hasFresnel;
	vec3 diffuse, ambient, specular;
};

uniform sampler2D texture0;

void main(void) {

	color = texture(texture0, out_TextureCoords);
	//color = vec4(out_Color, 1.0);
	//color = vec4(0.1, 0.2, 0.8, 1.0);
	//color = vec4(1.0, 1.0, 1.0, 1.0);

}
