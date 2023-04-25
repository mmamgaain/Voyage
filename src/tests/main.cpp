#include <voyage.hpp>
#include "Voyage/core.hpp"
#include "Voyage/game_action.hpp"
#include "Voyage/light.hpp"
#include "Voyage/model.hpp"
#include "Voyage/camera.hpp"
#include "Voyage/loader.hpp"
#include "Voyage/model_renderer.hpp"
#include "Voyage/particle_master.hpp"
#include "Voyage/particle_renderer.hpp"
#include "Voyage/particle_system.hpp"
#include "Voyage/particle_texture.hpp"
#include "Voyage/shader_program.hpp"
#include "Voyage/maths.hpp"
#include "Voyage/imgui_interface.hpp"
#include "Voyage/skybox.hpp"
#include "Voyage/terrain.hpp"
#include "Voyage/terrain_renderer.hpp"
#include "Voyage/material_terrain.hpp"
#include "Voyage/types.hpp"

using namespace Voyage;

class Main : public Core {
	public:
		Main() {
			#ifndef _DEBUG
			set_fullscreen(true);
			#endif
			set_vsync(GLFW_FALSE);
			init();
			// set_background_color(0.608, 0.104, 0.224);
			set_background_color(0.9922, 0.8784, 0.8509);
			hide_and_lock_cursor(true);

			models.reserve(4);
			// terrains.reserve(2);
			loadAssets();

			initControls();
			glm::mat4 proj{1.0};
			getPerspectiveProjectionMatrix(proj);

			SkyboxRenderer::init("res/shaders/sky.vs", "res/shaders/sky.fs", proj, loader, {"res/textures/skyboxes/default/right.png", "res/textures/skyboxes/default/left.png", "res/textures/skyboxes/default/top.png", "res/textures/skyboxes/default/bottom.png", "res/textures/skyboxes/default/back.png", "res/textures/skyboxes/default/front.png"});
			SkyboxRenderer::setSkyRotation(SKYBOX_ROTATION_DIRECTION::ANTICLOCKWISE);

			// Renderers
			renderer = new ModelRenderer("res/shaders/triangle_grad.vs", "res/shaders/triangle_grad.fs", proj);
			ImGuiInterface::init(window);

			tRenderer = new TerrainRenderer("res/shaders/terrain.vs", "res/shaders/terrain.fs", proj);
			ParticleMaster::init("res/shaders/particle.vs", "res/shaders/particle.fs", loader, proj);
			system = new ParticleSystem("res/shaders/particle.vs", "res/shaders/particle.fs", loader, 10, 2, 4, ParticleTexture(loader.loadTexture("res/textures/particles/cosmic.png"), 4, 4), 0.2F);
			system->setScaleError(2.0F);
			system->setSpeedError(1.0F);
			system->setLifeError(2.0F);
			system->setDirection({0.0F, 1.0F, 0.0F}, 2.0F);

			startGame();
		}

		void start() { startGame(); }

		void loadAssets() {
			// Models
			models.emplace_back("res/models/monkey.obj", loader, glm::vec3(10.0, 5.0, -20.0), glm::vec3(), glm::vec3(5));
			models.emplace_back("res/models/crate/crate.obj", loader, glm::vec3(-10.0, 3.0, -20.0), glm::vec3(22.6, 51.4, 32.2), glm::vec3(0.02, 0.02, 0.02));
			models.emplace_back("res/models/dragon.obj", loader, glm::vec3(0.0, 2.0, -30.0));
			models.emplace_back("res/models/PlantAgave002.fbx", loader, glm::vec3(-10.0, 5.0, -30.0), glm::vec3(-90.0, 0, 0), glm::vec3(5));
			// Terrains
			terrains.emplace_back(loader, Terrain::TerrainProps(400, 400, -200, 200), MaterialTerrain(loader, "res/textures/mud.png", "res/textures/grassFlowers.png", "res/textures/grass.png", "res/textures/mosaic-floor.jpg", "res/textures/blendmaps/blendMap.png"));
			// terrains.emplace_back(loader, Terrain::TerrainProps(400, 400, 400, 200), MaterialTerrain(loader, "res/textures/mud.png", "res/textures/grassFlowers.png", "res/textures/grass.png", "res/textures/mosaic-floor.jpg", "res/textures/blendmaps/blendMap.png"));
		}

		~Main() = default;

		void update(const double& deltaTime) override {
			handleControls();

			camera.getViewMatrix(view);

			for(const auto& model : models) renderer->render(model, view, light);
			tRenderer->render(terrains, view, light);
			SkyboxRenderer::render(view);
			system->generateParticles({0, 8, -25});
			updateImGui();
			ParticleMaster::render(view);
		}

		void dispose() override {
			ImGuiInterface::dispose();
			SAFE_DELETE(renderer);
			std::for_each(models.begin(), models.end(), [](Model& model) { model.dispose(); });
			SkyboxRenderer::dispose();
			ParticleMaster::dispose();
		}

	private:
		GameAction *exit, *forward, *backward, *left, *right, *panLeft, *panRight, *panUp, *panDown, *up, *down;
		Loader loader;
		ModelRenderer *renderer;
		glm::mat4 view;
		Camera1P camera{{0, 5, 0}, 0, 0, 0};
		std::vector<Model> models;
		DirectionalLight light = DirectionalLight({100, 5000, 100});
		TerrainRenderer *tRenderer;
		std::vector<Terrain> terrains;
		ParticleSystem *system;

		float MOVE_SPEED = 0.1, ROT_SPEED = 0.01;

		void updateImGui() {
			ImGuiInterface::startFrame();
			ImGuiInterface::startWindow();
			// ImGui::Text("This is some useful text.\nMayank is awesome.");
			ImGui::Text("FPS: %lf", 1.0/deltaTime);
			ImGui::SliderFloat("Pitch", &camera.pitch, -180, 180);
			ImGui::SliderFloat("Yaw", &camera.yaw, -180, 180);
			ImGui::SliderFloat3("Camera position", &camera.position[0], -50, 50);
			ImGui::Text("Particles: %d", ParticleMaster::getNumParticles(&system->texture));
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
	Main main_instance;
	// main_instance.start();
	return 0;
}
