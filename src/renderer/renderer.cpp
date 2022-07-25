#include "renderer.hpp"
#include <glad.h>

namespace Cyclone {

// Renderer member declarations start
int Renderer::MAX_BUFFER_SIZE = -1;

	void Renderer::loadTexture2D(const int& index, const int& textureID) { glActiveTexture(GL_TEXTURE0 + index); glBindTexture(GL_TEXTURE_2D, textureID); }
	
	void Renderer::loadTexture3D(const int& index, const int& textureID) { glActiveTexture(GL_TEXTURE0 + index); glBindTexture(GL_TEXTURE_3D, textureID); }
	
	void Renderer::loadTextureCubeMap(const int& index, const int& textureID) { glActiveTexture(GL_TEXTURE0 + index); glBindTexture(GL_TEXTURE_CUBE_MAP, textureID); }
	
	void Renderer::renderTriangle(RawModel& model) const {
		prepareRender(model);
		drawTriangleCall(model);
		finishRender(model);
	}
	
	void Renderer::prepareRender(RawModel& model) const {
		if(MAX_BUFFER_SIZE == -1) glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &MAX_BUFFER_SIZE);
		glBindVertexArray(model.getID());
		if(model.isValidNumBuffers()) for(size_t i = 0; i < model.getNumBuffers(); i++) glEnableVertexAttribArray(i);
		else {
			for(size_t i = 0; i < MAX_BUFFER_SIZE; i++) {
				int enabled = GL_FALSE;
				glGetVertexAttribiv(i, GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING, &enabled);
				if(enabled != GL_FALSE) glEnableVertexAttribArray(i);
				else { model.setNumBuffers(i); break; }
			}
		}
	}

	void Renderer::finishRender(const RawModel& model) const {
		for(size_t i = 0; i < model.getNumBuffers(); i++) glDisableVertexAttribArray(i);
		glBindVertexArray(0);
	}
	
	void Renderer::drawTriangleCall(const RawModel& model) const {
		if(model.hasIndexBuffer()) glDrawElements(GL_TRIANGLES, model.getVertexCount(), GL_UNSIGNED_INT, nullptr);
		else glDrawArrays(GL_TRIANGLE_STRIP, 0, model.getVertexCount());
	}
// Renderer member declarations ends



// TexturedRenderer member declarations start

// TexturedRenderer member declarations ends

}
