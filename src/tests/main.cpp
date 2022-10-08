#include <voyage.hpp>
#include "extra/core/core.hpp"
#include "extra/input/game_action.hpp"
#include "extra/light.hpp"
#include "extra/models/model.hpp"
#include "extra/camera.hpp"
#include "extra/loader.hpp"
#include "extra/texture/material_terrain.hpp"
#include "renderer/model_renderer.hpp"
#include "shaders/shader_program.hpp"
#include "extra/maths.hpp"
#include "extra/imgui_interface.hpp"
#include "extra/types.hpp"
#include "renderer/skybox.hpp"
#include "extra/models/terrain.hpp"
#include "renderer/terrain_renderer.hpp"

#define IS_LAPTOP 1

using namespace Voyage;

class Main : public Core {
	public:
	Main() {
		if(!is_debug) {
			if(IS_LAPTOP) set_screen_resolution(1366, 768);
			else set_screen_resolution(1920, 1080);
			set_fullscreen(true);
		}
		else { set_screen_resolution(600, 400); }
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
		dragon = new Model("res/models/dragon.obj", loader, {0.0, 0.0, -20.0}, glm::vec3(0), glm::vec3(1.0));

		// Renderers
		renderer = new ModelRenderer("res/shaders/triangle_grad.vs", "res/shaders/triangle_grad.fs", proj);
		SkyboxRenderer::init("res/shaders/sky.vs", "res/shaders/sky.fs", proj, loader, {"res/textures/skyboxes/default/right.png", "res/textures/skyboxes/default/left.png", "res/textures/skyboxes/default/top.png", "res/textures/skyboxes/default/bottom.png", "res/textures/skyboxes/default/back.png", "res/textures/skyboxes/default/front.png"});
		terrenderer = new TerrainRenderer("res/shaders/terrain.vs", "res/shaders/terrain.fs", proj);
		SkyboxRenderer::setSkyRotation(ANTICLOCKWISE, 0.5F);
		ImGuiInterface::init(window);

		light = new DirectionalLight({100, 5000, 100});

		startGame();
	}

	~Main() = default;

	void update(double deltaTime) override {
		// stopGame();
		handleControls();

		camera.getViewMatrix(view);

		renderer->render(*dragon, view, *light);
		terrenderer->render(terrains, view, *light);
		SkyboxRenderer::render(view);

		updateImGui();
	}

	void dispose() override {
		ImGuiInterface::dispose();
		SAFE_DELETE(dragon);
		SAFE_DELETE(light);
		SkyboxRenderer::dispose();
		SAFE_DELETE(terrenderer);
		SAFE_DELETE(renderer);
	}

	private:
	GameAction *exit, *forward, *backward, *left, *right, *panLeft, *panRight, *panUp, *panDown, *up, *down;
	Loader loader;
	ModelRenderer *renderer;
	glm::mat4 view;
	Camera1P camera{{0, 5, 0}, 0, 0, 0};
	Model *dragon;
	DirectionalLight *light;
	std::vector<Terrain> terrains;
	TerrainRenderer *terrenderer;

	float MOVE_SPEED = 0.1, ROT_SPEED = 0.01;

	void updateImGui() {
		ImGuiInterface::startFrame();
		ImGuiInterface::startWindow();
		ImGui::Text("This is some useful text.\nMayank is awesome.");
		// ImGui::LabelText("FPS", deltaTime);
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
