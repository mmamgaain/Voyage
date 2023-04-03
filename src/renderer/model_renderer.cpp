#include "Voyage/model_renderer.hpp"
#include "Voyage/maths.hpp"
#include "Voyage/raw_model.hpp"

namespace Voyage {
	ModelRenderer::ModelRenderer(const char* const vertex_file, const char* const fragment_file, const glm::mat4& projection) {
		shader = new ShaderProgram(vertex_file, fragment_file, 3);
		shader->start();
		shader->loadUniform("projection", projection);
		shader->remapTextureSampleName(0, "material.texture0");
		shader->remapTextureSampleName(1, "material.normalMap");
		shader->remapTextureSampleName(2, "material.specularMap");
		shader->stop();
	}

	ModelRenderer::~ModelRenderer() { dispose(); }

	void ModelRenderer::render(const Model& model, const glm::mat4& view, DirectionalLight& light) {
		shader->start();
		model.getTransformation(transform);
		shader->loadUniform("model", transform);
		shader->loadUniform("view", view);
		shader->loadUniform("lightDir", light.getDirection());
		shader->loadUniform("lightCol", light.getColor());
		loadMaterial(model.getMaterial());
		for(const auto& mod : model.getModels()) renderer.renderTriangle(mod.get());
		shader->stop();
	}

	void ModelRenderer::dispose() { if(shader) delete shader; }

	void ModelRenderer::loadMaterial(const Material& material) {
		shader->loadUniform("material.ambient", material.getAmbientColor());
		shader->loadUniform("material.diffuse", material.getDiffuseColor());
		shader->loadUniform("material.specular", material.getSpecularColor());
		shader->loadUniform("material.reflectivity", material.specularReflectivity);
		shader->loadUniform("material.shineDamper", material.shineDamper);
		shader->loadUniform("material.hasDiffuseTexture", material.hasTexture());
		shader->loadUniform("material.hasNormalMap", material.hasNormalMap());
		shader->loadUniform("material.hasSpecularMap", material.hasSpecularMap());
		shader->loadUniform("material.isTransparent", (bool) material.transparency);
		shader->loadUniform("material.transparency", material.transparency);
		shader->loadUniform("material.hasFresnel", material.hasFresnel);
		shader->loadUniform("material.fresnelPower", material.fresnelPower);
	}
}
