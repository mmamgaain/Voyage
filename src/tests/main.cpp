#include <voyage.hpp>
#include "Voyage/core.hpp"
#include "Voyage/game_action.hpp"
#include "Voyage/light.hpp"
#include "Voyage/model.hpp"
#include "Voyage/camera.hpp"
#include "Voyage/loader.hpp"
#include "Voyage/material_terrain.hpp"
#include "Voyage/model_renderer.hpp"
#include "Voyage/shader_program.hpp"
#include "Voyage/maths.hpp"
#include "Voyage/imgui_interface.hpp"
#include "Voyage/types.hpp"
#include "Voyage/skybox.hpp"
#include "Voyage/terrain.hpp"
#include "Voyage/terrain_renderer.hpp"
#include "Voyage/particle_master.hpp"
#include "Voyage/source.hpp"
#include "Voyage/audio.hpp"

using namespace Voyage;

class Main : public Core {
	public:
		Main() {
			#ifndef _DEBUG
				set_fullscreen(true);
			#endif
			// set_vsync(GLFW_FALSE);
			init();
			// set_background_color(0.608, 0.104, 0.224);
			set_background_color(0.9922, 0.8784, 0.8509);
			hide_and_lock_cursor(true);

			initControls();
			glm::mat4 proj{1.0};
			getPerspectiveProjectionMatrix(proj);

			// Models
			terrains.emplace_back(loader, Terrain::TerrainProps(500, 500, 0, 0, -2), MaterialTerrain(loader, "res/textures/mud.png", "res/textures/grass.png", "res/textures/grass.jpg", "res/textures/mosaic-floor.jpg", "res/textures/blendmaps/blendMap.png"));
			dragon = new Model("res/models/monkey.obj", loader, {10.0, 5.0, -20.0}, {}, glm::vec3(5));
			crate = new Model("res/models/crate/crate.obj", loader, {-10.0, 3.0, -20.0}, {22.6, 51.4, 32.2}, {0.02, 0.02, 0.02});

			// Renderers
			renderer = new ModelRenderer("res/shaders/triangle_grad.vs", "res/shaders/triangle_grad.fs", proj);
			SkyboxRenderer::init("res/shaders/sky.vs", "res/shaders/sky.fs", proj, loader, {"res/textures/skyboxes/default/right.png", "res/textures/skyboxes/default/left.png", "res/textures/skyboxes/default/top.png", "res/textures/skyboxes/default/bottom.png", "res/textures/skyboxes/default/back.png", "res/textures/skyboxes/default/front.png"});
			terrenderer = new TerrainRenderer("res/shaders/terrain.vs", "res/shaders/terrain.fs", proj);
			SkyboxRenderer::setSkyRotation(ANTICLOCKWISE, 0.5F);
			ImGuiInterface::init(window);

			light = new DirectionalLight({100, 5000, 100});
			ParticleMaster::init("res/shaders/particle.vs", "res/shaders/particle.fs", loader, proj);
			AudioMaster::init();
			sound = new Source(AudioMaster::loadSound("res/music/Idyllic_Memories.mp3"));
			gunshot = new Source(AudioMaster::loadSound("res/music/gunshot_7.wav"));
			sound->play();

			startGame();
		}

		~Main() = default;

		void update(double deltaTime) override {
			// stopGame();
			handleControls();

			camera.getViewMatrix(view);

			renderer->render(*dragon, view, *light);
			renderer->render(*crate, view, *light);
			terrenderer->render(terrains, view, *light);
			SkyboxRenderer::render(view);
			ParticleMaster::update();
			ParticleMaster::render(view);

			updateImGui();
		}

		void dispose() override {
			ImGuiInterface::dispose();
			SAFE_DELETE(dragon);
			SAFE_DELETE(light);
			SkyboxRenderer::dispose();
			SAFE_DELETE(terrenderer);
			SAFE_DELETE(renderer);
			ParticleMaster::dispose();
			AudioMaster::dispose();
			delete sound;
		}

	private:
		GameAction *exit, *forward, *backward, *left, *right, *panLeft, *panRight, *panUp, *panDown, *up, *down, *fire;
		Loader loader;
		ModelRenderer *renderer;
		glm::mat4 view;
		Camera1P camera{{0, 5, 0}, 0, 0, 0};
		Model *dragon, *crate;
		DirectionalLight *light;
		std::vector<Terrain> terrains;
		TerrainRenderer *terrenderer;
		Source *sound, *gunshot;

		float MOVE_SPEED = 0.1, ROT_SPEED = 0.01;

		void updateImGui() {
			ImGuiInterface::startFrame();
			ImGuiInterface::startWindow();
			ImGui::Text("This is some useful text.\nMayank is awesome.");
			// ImGui::Text("FPS: %lf", 1.0/deltaTime);
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
			fire = GameAction::add_key_bind(GLFW_KEY_SPACE, GAME_INPUT_BEHAVIOUR_DETECT_INITIAL_PRESS_ONLY);
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
			if(fire->isPressed()) {
				ParticleMaster::addParticle(camera.getPosition() + glm::vec3(0.0, -0.2, 0.0), {glm::sin(camera.getYaw()), camera.position[1], -glm::cos(camera.getYaw())}, {0.3, 0.3, 0.3}, 2.0, 3.0, 48.0);
				gunshot->play();
			}
		}
};

int main(int argc, char** argv) {
	Main main_instance;

	return 0;

}
