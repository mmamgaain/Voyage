#include "cards_renderer.hpp"

namespace Voyage {

	CardsRenderer::CardsRenderer(const char* vertex_file, const char* fragment_file) {
		shader = new ShaderProgram(vertex_file, fragment_file, 1);
	}

	CardsRenderer::~CardsRenderer() { delete shader; }

	void CardsRenderer::render(const Cards& card) {
		const RawModel& model = card.getModel();
		shader->start();
		renderer.prepareRender(model);
		renderer.loadTexture2D(0, card.getTextureID());
		card.getTransformation(transform);
		shader->loadUniform("transform", transform);
		renderer.drawTriangleCall(model);
		renderer.finishRender(model);
		shader->stop();
	}

}

