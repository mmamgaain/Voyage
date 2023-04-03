#include "Voyage/renderer.hpp"
#include "Voyage/raw_model.hpp"

namespace Voyage {

	int Renderer::MAX_BUFFER_SIZE = -1;

	void Renderer::loadTexture2D(const int& index, const int& textureID) const { glActiveTexture(GL_TEXTURE0 + index); glBindTexture(GL_TEXTURE_2D, textureID); }

	void Renderer::loadTexture3D(const int& index, const int& textureID) const { glActiveTexture(GL_TEXTURE0 + index); glBindTexture(GL_TEXTURE_3D, textureID); }

	void Renderer::loadTextureCubeMap(const int& index, const int& textureID) const { glActiveTexture(GL_TEXTURE0 + index); glBindTexture(GL_TEXTURE_CUBE_MAP, textureID); }

	void Renderer::enableBackCulling() { glEnable(GL_CULL_FACE); glCullFace(GL_BACK); }

	void Renderer::disableBackCulling() { glDisable(GL_CULL_FACE); }

	void Renderer::renderTriangle(const RawModel* model) const {
		prepareRender(model);
		drawTriangleCall(model);
		finishRender(model);
	}

	void Renderer::prepareRender(const RawModel* const model) const {
		if(MAX_BUFFER_SIZE == -1) glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &MAX_BUFFER_SIZE);

		glBindVertexArray(model->getID());
		if(model->isValidNumBuffers()) for(size_t i = 0; i < model->getNumBuffers(); i++) glEnableVertexAttribArray(i);
		else {
			std::vector<uint32_t> vbos;
			int enabled;
			for(int i = 0; i < MAX_BUFFER_SIZE; i++) {
				glGetVertexAttribiv(i, GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING, &enabled);
				if(enabled != GL_FALSE) { glEnableVertexAttribArray(i); vbos.push_back(i); }
				else { model->_setVBOs(vbos); break; }
			}
		}
	}

	void Renderer::finishRender(const RawModel* const model) const {
		for(size_t i = 0; i < model->getNumBuffers(); i++) glDisableVertexAttribArray(i);
		glBindVertexArray(0);
	}

	void Renderer::drawTriangleCall(const RawModel* const model) const {
		if(model->hasIndexBuffer()) glDrawElements(GL_TRIANGLES, model->getVertexCount(), GL_UNSIGNED_INT, nullptr);
		else glDrawArrays(GL_TRIANGLE_STRIP, 0, model->getVertexCount());
	}

}
