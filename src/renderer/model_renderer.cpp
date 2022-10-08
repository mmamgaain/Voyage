#include "model_renderer.hpp"
#include "extra/maths.hpp"

namespace Voyage {
	ModelRenderer::ModelRenderer(const char* const vertex_file, const char* const fragment_file, const glm::mat4& projection) {
		shader = new ShaderProgram(vertex_file, fragment_file);
		shader->start();
		shader->loadUniform("projection", projection);
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
		shader->loadUniform("reflectivity", 1);
		shader->loadUniform("shineDamper", 10);
		for(auto mod : model.getModels()) renderer.renderTriangle(mod);
		shader->stop();
	}

	void ModelRenderer::dispose() { if(shader) delete shader; }
}
