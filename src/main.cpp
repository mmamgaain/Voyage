#include <voyage.hpp>
#include "extra/fbo.hpp"
#include "extra/core/core.hpp"
#include "extra/input/game_action.hpp"
#include "extra/models/model.hpp"
#include "extra/models/raw_model.hpp"
#include "extra/camera.hpp"
#include "extra/loader.hpp"
#include "extra/types.hpp"
#include "extra/texture/texture.hpp"
#include "shaders/shader_program.hpp"
#include "renderer/renderer.hpp"
#include "extra/maths.hpp"
#include "extra/imgui_interface.hpp"
#include "extra/types.hpp"

// #define X_DISPLACMENT 2.0
// #define Y_DISPLACMENT 1.0
// #define Z_DISPLACMENT 1.0
#define X_DISPLACMENT 9.0
#define Y_DISPLACMENT 9.0
#define Z_DISPLACMENT 5.0

#define X_LOW  -X_DISPLACMENT
#define X_HIGH  X_DISPLACMENT
#define Y_LOW  -Y_DISPLACMENT
#define Y_HIGH  Y_DISPLACMENT
#define Z_LOW   Z_DISPLACMENT
#define Z_HIGH -Z_DISPLACMENT

#define IS_LAPTOP 1

using namespace Cyclone;

std::vector<float> VERTICES = {
	X_LOW,  Y_HIGH, Z_LOW,
	X_LOW,  Y_LOW,  Z_LOW,
	X_HIGH, Y_HIGH, Z_LOW,
	X_HIGH, Y_LOW,  Z_LOW,
	X_LOW,  Y_HIGH, Z_HIGH,
	X_LOW,  Y_LOW,  Z_HIGH,
	X_HIGH, Y_HIGH, Z_HIGH,
	X_HIGH, Y_LOW,  Z_HIGH
};

std::vector<unsigned int> INDICES = {
	0, 1, 2, 	2, 1, 3,
	2, 3, 6,	6, 3, 7,
	6, 7, 4,	4, 7, 5,
	4, 5, 0,	0, 5, 1,
	4, 0, 6,	6, 0, 2,
	1, 5, 3,	3, 5, 7
};

class Main : public Core {

	public:
		std::unique_ptr<RawModel> model;
		Main() {
			if(!is_debug) {
				// true for laptop
				if(IS_LAPTOP) set_screen_resolution(1366, 768);
				else set_screen_resolution(1920, 1080);
				set_fullscreen(true);
			}
			else { set_screen_resolution(600, 400); }
			// set_vsync(GLFW_FALSE);
			init();
			// set_background_color(0.9922, 0.8784, 0.8509);
			set_background_color(0, 0, 0);
			hide_and_lock_cursor(true);

			initControls();
			model = loader.loadToVAO(VERTICES, 3, INDICES);
			shader = new ShaderProgram("res/shaders/triangle_grad.vs", "res/shaders/triangle_grad.fs");
			glm::mat4 proj{1.0};
			getPerspectiveProjectionMatrix(proj);
			shader->start();
			shader->loadUniform("projection", proj);
			shader->stop();

			getTransformationMatrix(transform, {0.0, 0.0, -20.0}, glm::vec3(), glm::vec3(1.0));

			ImGuiInterface::init(window);
			texture = loader.loadTexture("res/textures/dragon.png");
			dragon = new Model("res/models/dragon.obj", loader);

			startGame();
		}

		~Main() = default;

		void update(double deltaTime) override {
			handleControls();
			updateImGui();

			shader->start();
			shader->loadUniform("model", transform);
			shader->loadUniform("view", camera.getViewMatrix());
			renderer.loadTexture2D(0, texture->getID());
			// renderer.renderTriangle(*model);
			renderer.renderTriangle(dragon->getModels()[0]);

			shader->stop();
		}

		void dispose() override {
			loader.dispose();
			shader->dispose();
			delete shader;
			ImGuiInterface::dispose();
			delete dragon;
		}

	private:
		GameAction *exit, *forward, *backward, *left, *right, *panLeft, *panRight, *panUp, *panDown, *up, *down;
		ShaderProgram* shader;
		Loader loader;
		Renderer renderer;
		glm::mat4 transform{1.0};
		Camera1P camera{{0, 5, 0}};
		Model* dragon;
		Texture* texture;
		float MOVE_SPEED = 0.1, ROT_SPEED = 0.01;
		/* const std::vector<float> VERTICES{
		   -0.5,  0.5, 0.0,
		   -0.5, -0.5, 0.0,
		   0.5,  0.5, 0.0,
		   0.5, -0.5, 0.0
		   };

		   const std::vector<unsigned int> INDICES{
		   0, 1, 2,
		   2, 1, 3
		   }; */

		void updateImGui() {
			ImGuiInterface::startFrame();

			ImGuiInterface::startWindow();
			ImGui::Text("This is some useful text.\nMayank is awesome.");
			ImGui::SliderFloat("Pitch", &camera.pitch, -180, 180);
			ImGui::SliderFloat("Yaw", &camera.yaw, -180, 180);
			ImGui::SliderFloat3("Camera position", &camera.position[0], -50, 50);
			ImGuiInterface::endWindow();

			ImGuiInterface::endFrame();

			ImGuiInterface::render();
		}

		void initControls() {
			exit = GameAction::add_key_bind(GLFW_KEY_ESCAPE);
			forward = GameAction::add_key_bind(GLFW_KEY_W);
			backward = GameAction::add_key_bind(GLFW_KEY_S);
			left = GameAction::add_key_bind(GLFW_KEY_A);
			right = GameAction::add_key_bind(GLFW_KEY_D);
			up = GameAction::add_key_bind(GLFW_KEY_UP);
			down = GameAction::add_key_bind(GLFW_KEY_DOWN);
			panLeft = GameAction::add_mouse_bind(MM_LEFT);
			panRight = GameAction::add_mouse_bind(MM_RIGHT);
			panUp = GameAction::add_mouse_bind(MM_UP);
			panDown = GameAction::add_mouse_bind(MM_DOWN);
		}

		void handleControls() {
			ROT_SPEED = deltaTime;
			MOVE_SPEED = deltaTime * 6;
			if(exit->isPressed()) stopGame();
			if(forward->isPressed()) camera.moveForward(MOVE_SPEED);
			if(backward->isPressed()) camera.moveBackward(MOVE_SPEED);
			if(left->isPressed()) camera.moveLeft(MOVE_SPEED);
			if(right->isPressed()) camera.moveRight(MOVE_SPEED);
			if(up->isPressed()) camera.position.y += MOVE_SPEED;
			if(down->isPressed()) camera.position.y -= MOVE_SPEED;
			camera.panLeft(ROT_SPEED * panLeft->getAmount());
			camera.panRight(ROT_SPEED * panRight->getAmount());
			camera.panUp(ROT_SPEED * panUp->getAmount());
			camera.panDown(ROT_SPEED * panDown->getAmount());
		}

};

int main(int argc, char** argv) {
	if(argv[1]) Core::is_debug = !strcmp(argv[1], "debug");
	Main main_instance;

	return 0;

}
