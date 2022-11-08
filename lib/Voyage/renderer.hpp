#pragma once
#include <voyage.hpp>
#include "Voyage/raw_model.hpp"

namespace Voyage {

	class Renderer {
		public:
			Renderer() noexcept = default;

			~Renderer() noexcept = default;

			void loadTexture2D(const int& index, const int& textureID) const;

			void loadTexture3D(const int& index, const int& textureID) const;

			void loadTextureCubeMap(const int& index, const int& textureID) const;

			static void enableBackCulling();

			static void disableBackCulling();

			virtual void renderTriangle(RawModel& model) const;

			virtual void prepareRender(const RawModel& model) const;

			virtual void finishRender(const RawModel& model) const;

			virtual void drawTriangleCall(const RawModel& model) const;

		private:
			static int MAX_BUFFER_SIZE;

	};

}
