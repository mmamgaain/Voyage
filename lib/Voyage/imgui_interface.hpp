#pragma once
#include <voyage.hpp>

namespace Voyage {

	class ImGuiInterface {
		public:
			/* Initialises the ImGui pipeline. This means that this function needs to be called *ONLY ONCE* during the entire lifetime of the application.
			 * The only scenario where this function might need to be called again during the run of the application is if dispose was called and the ImGui interface needs to be initialised again. */
			static void init(GLFWwindow* window);

			static void sliderMat4(const char* label, glm::mat4 mat, float min, float max);

			static void startFrame();

			static void endFrame();

			static void startWindow(const char* name = "Voyage Engine Panel");

			static void endWindow();

			static void render();

			static void dispose();
		private:
	};

}
