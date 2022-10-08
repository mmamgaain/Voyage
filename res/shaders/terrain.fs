#version 330 core

in vec2 pass_textureCoords;
in vec3 surfaceNormal, toLightVector, toCameraVector;
in float visibility;

out vec4 color;

struct Material {
	sampler2D diffuse, red, green, blue, blendMap, specularMap;
	int textureIndex;
	float shineDamper, reflectivity, tiling;
	bool multitextured, hasSpecularMap;
};
uniform Material material;

uniform float time, ambientLightIntensity;
uniform vec3 lightColor, skyColor, lightAttenuation;

void main(void){

	vec2 out_textureCoords = pass_textureCoords * material.tiling;

	// Phong shading
	vec3 unitLightVector = normalize(toLightVector),
		 unitCameraVector = normalize(toCameraVector),
		 unitNormal = normalize(surfaceNormal);
	float brightness = max(dot(unitNormal, unitLightVector), ambientLightIntensity);
	float lightDist = length(toLightVector),
		  attenuation = lightAttenuation.x + lightAttenuation.y * lightDist + lightAttenuation.z * lightDist * lightDist;
	vec3 diffuse = brightness * lightColor / attenuation;

	vec4 textureColor = vec4(0.0);

	// Diffuse shading
	if(material.multitextured){
		vec4 blendMapColor = texture(material.blendMap, pass_textureCoords);
		float backgroundColor = 1.0 - (blendMapColor.r + blendMapColor.g + blendMapColor.b);
		vec4 backgroundTexture = texture(material.diffuse, out_textureCoords) * backgroundColor;
		vec4 redTexture = texture(material.red, out_textureCoords) * blendMapColor.r,
			 greenTexture = texture(material.green, out_textureCoords) * blendMapColor.g,
			 blueTexture = texture(material.blue, out_textureCoords) * blendMapColor.b;

		textureColor = backgroundTexture + redTexture + greenTexture + blueTexture;
	}
	else textureColor = texture(material.diffuse, out_textureCoords);

	// Specular shading
	vec3 reflectedLightVector = reflect(-unitLightVector, unitNormal);
	float specularFactor = max(dot(reflectedLightVector, unitCameraVector), 0.0),
		  dampedFactor = pow(specularFactor, material.shineDamper) * material.reflectivity;
	vec3 finalSpecular = dampedFactor * lightColor;
	if(material.hasSpecularMap) {
		vec3 specularMapColor = texture(material.specularMap, out_textureCoords).rgb;
		finalSpecular *= specularMapColor.r + specularMapColor.b;
		if(specularMapColor.g > 0.5) diffuse = vec3(1.0);
	}
	finalSpecular = vec3(0.0);
	// finalSpecular *= material.specular / attenuation;

	color = textureColor * vec4(diffuse, 1.0) + vec4(finalSpecular, 1.0);
	color = mix(vec4(skyColor, 1.0), color, visibility);

	// EXPERIMENTAL : HDR lighting
	// float exposure = 0.1;
	// vec3 tonemappedColor = 1.0 - exp(-color.rgb * exposure);
	// color.rgb = pow(tonemappedColor, vec3(1.0 / 2.2));

	// vec3 reflectedLightVector = reflect(-unitLightVector, unitNormal);
	// float specularFactor = max(dot(reflectedLightVector, toCameraVector), 0.0),
	// 	  dampedFactor = pow(specularFactor, material.shineDamper) * material.reflectivity;
	// vec3 finalSpecular = dampedFactor * lightColor;
	// color = vec4( vec3(specularFactor) , 1.0);


}
