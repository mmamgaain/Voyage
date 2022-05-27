#include "../lib/glad.h"
#include <iostream>
#include "core/core.hpp"
#include "input/game_action.hpp"
#include "models/raw_model.hpp"
#include "utils/loader.hpp"
#include "../lib/glfw3.h"
#include "shaders/shader_program.hpp"
#include <memory>
#include <string>
#include <string.h>
#include <vector>

using namespace Cyclone;

class Main : public Core {

	private:
		GameAction *exit, *mouse_pos_print;
		Loader loader;
		ShaderProgram* shader;
		const std::vector<float> VERTICES{
			-0.5,  0.5, 0.5,
			-0.5, -0.5, 0.5,
			 0.5,  0.5, 0.5,
			 0.5, -0.5, 0.5
		};

		const std::vector<unsigned int> INDICES{
			0, 1, 2,
			2, 1, 3
		};

		void handleControls() {
			if(exit->is_pressed()) stopGame();
			if(mouse_pos_print->is_pressed()) std::cout << "Mouse X : " << mouseX << ", Mouse Y : " << mouseY << std::endl;
		}

	public:
		std::unique_ptr<RawModel> model;
		Main() {
			std::cout << "Main created!" << std::endl;
			if(!is_debug) {
				// true for laptop
				if(false) set_screen_resolution(1366, 768);
				else set_screen_resolution(1920, 1080);
				set_fullscreen(true);
			}
			else { set_screen_resolution(600, 400); }
			// set_vsync(GLFW_FALSE);
			init();
			set_background_color(0.9922, 0.8784, 0.8509, 1.0);
			hide_and_lock_cursor(true);

			exit = GameAction::add_key_bind(GLFW_KEY_ESCAPE, GAME_INPUT_BEHAVIOUR_DETECT_INITIAL_PRESS_ONLY, "EXIT");
			mouse_pos_print = GameAction::add_mouse_bind(MB_LEFT, GAME_INPUT_BEHAVIOUR_NORMAL, "CLICKITY CLACK");
			// std::cout << *exit << std::endl;
			// std::cout << *mouse_pos_print << std::endl;
			model = loader.loadToVAO(VERTICES, 3, INDICES);
			shader = new ShaderProgram("/home/mayank/Programming/Cyclone_cpp/res/shaders/triangle_grad.vs", "/home/mayank/Programming/Cyclone_cpp/res/shaders/triangle_grad.fs");

			std::cout << "Vertex Count : " << model->getVertexCount() << std::endl;

			startGame();
		}

		~Main() { std::cout << "Main destroyed!" << std::endl; }

		void update(double deltaTime) override {
			handleControls();

			shader->start();
			glBindVertexArray(model->getID());
			for(size_t i = 0; i < model->getNumBuffers(); i++) glEnableVertexAttribArray(i);
			// set_background_color(0.2706, 0.2941, 0.1059, 1.0);
			// glDrawArrays(GL_TRIANGLES, 0, model->getVertexCount());
			glDrawElements(GL_TRIANGLES, model->getVertexCount(), GL_UNSIGNED_INT, nullptr);
			for(size_t i = 0; i < model->getNumBuffers(); i++) glDisableVertexAttribArray(i);
			glBindVertexArray(0);
			shader->stop();
			// set_background_color(0.9922, 0.8784, 0.8509, 1.0);
		}

		void dispose() override {
			loader.dispose();
			shader->dispose();
			delete shader;
			std::cout << "Model ID : " << model->getID() << std::endl;
		}

};

void intermediateFunc() {
	Main main_instance;
	// main_instance->~Main();
	// delete main_instance;
}

int main(int argc, char** argv) {
	if(argv[1]) Core::is_debug = !strcmp(argv[1], "debug");
	intermediateFunc();
	return 0;
}

