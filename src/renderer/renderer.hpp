#pragma once
#include <voyage.hpp>
#include "../extra/models/raw_model.hpp"

namespace Cyclone {
	
	class Renderer {
		public:
			Renderer() = default;
			
			~Renderer() = default;

			void loadTexture2D(const int& index, const int& textureID);

			void loadTexture3D(const int& index, const int& textureID);

			void loadTextureCubeMap(const int& index, const int& textureID);

			void renderTriangle(RawModel& model) const;

		protected:
			void prepareRender(RawModel& model) const;

			void finishRender(const RawModel& model) const;

			void drawTriangleCall(const RawModel& model) const;

		private:
			static int MAX_BUFFER_SIZE;
	};

	class TexturedRenderer: Renderer {
		public:
			TexturedRenderer();
		private:

	};

}
