#include "Voyage/terrain_renderer.hpp"
#include "Voyage/maths.hpp"
#include "Voyage/imgui_interface.hpp"
#include "Voyage/terrain.hpp"
#include "Voyage/core.hpp"

namespace Voyage {

	TerrainRenderer::TerrainRenderer(const char* const vertex_file, const char* const fragment_file, const glm::mat4& projection): shader(new ShaderProgram(vertex_file, fragment_file, 5)), modelMatrix(glm::mat4(1.0)) {
		shader->start();
		shader->loadUniform("project", projection);
		shader->remapTextureSampleName(0, "material.diffuse");
		shader->remapTextureSampleName(1, "material.red");
		shader->remapTextureSampleName(2, "material.green");
		shader->remapTextureSampleName(3, "material.blue");
		shader->remapTextureSampleName(4, "material.blendMap");
		shader->stop();
	}

	TerrainRenderer::~TerrainRenderer() { dispose(); }

	void TerrainRenderer::loadMaterial(const MaterialTerrain& material) const {
		renderer.loadTexture2D(0, material.getDiffuseID());
		renderer.loadTexture2D(1, material.getRedID());
		renderer.loadTexture2D(2, material.getGreenID());
		renderer.loadTexture2D(3, material.getBlueID());
		renderer.loadTexture2D(4, material.getBlendMapID());
		shader->loadUniform("material.shineDamper", material.shineDamper);
		shader->loadUniform("material.reflectivity", material.reflectivity);
		shader->loadUniform("material.tiling", 80.0F);
		shader->loadUniform("material.multitextured", material.isMultitextured());
		shader->loadUniform("material.hasSpecularMap", false);
	}

	void TerrainRenderer::prepareRender(const Terrain& model) const {
		renderer.prepareRender(model.getptr_const());
		getTransformationMatrix(modelMatrix, {model.getX(), model.getHeight(), model.getZ()});
		shader->loadUniform("transform", modelMatrix);
		shader->loadUniform("clipPlane", 0, -1, 0, model.getHeight());
		shader->loadUniform("fogDensity", model.getProperties().fog_density);
		shader->loadUniform("fogGradient", model.getProperties().fog_gradient);
		loadMaterial(model.getMaterial());
	}

	void TerrainRenderer::render(const std::vector<Terrain>& model, const glm::mat4& view, const DirectionalLight& light) const {
		shader->start();
		shader->loadUniform("view", view);
		shader->loadUniform("ambientLightIntensity", 0.2F);
		shader->loadUniform("lightColor", light.getColor());
		shader->loadUniform("lightAttenuation", light.getAttenuation());
		shader->loadUniform("skyColor", 1.0, 0.0, 0.0);
		shader->loadUniform("lightPos", light.getDirection());
		for(const Terrain& terrain : model) {
			prepareRender(terrain);
			renderer.drawTriangleCall(terrain.getptr_const());
		}
		shader->stop();
	}

	void TerrainRenderer::dispose() { if(shader) { delete shader; shader = nullptr; } }

}
