#include "game_action.hpp"
#include "../../lib/glfw3.h"
#include "../core/core.hpp"
#include <iostream>

using namespace Cyclone;

std::vector<GameAction*> GameAction::keys;
GameAction* GameAction::mouses[9];

// Main constructor
GameAction::GameAction(const int key_code, const BEHAVIOURS behaviour, const std::string& name): amount(0), key(key_code), name(name), behaviour(behaviour), state(GAME_INPUT_STATE_RELEASED) {}

// Copy constructor
GameAction::GameAction(const GameAction& game_action): amount(game_action.amount), key(game_action.key), name(game_action.name), behaviour(game_action.behaviour), state(game_action.state) { std::cout << "GameAction copied!" << std::endl; }

// Move constructor
GameAction::GameAction(GameAction&& game_action): amount(std::move(game_action.amount)), key(std::move(game_action.key)), behaviour(std::move(game_action.behaviour)), state(std::move(game_action.state)) { std::cout << "GameAction Moved!" << std::endl; name = game_action.name; }

// Destructor
GameAction::~GameAction() { std::cout << "GameAction " << this->name << " with key " << this->key << " is being destroyed" << std::endl; }

GameAction* GameAction::add_key_bind(const int key_code, const BEHAVIOURS behaviour, const std::string& name) {
	std::cout << "Adding key bind" << std::endl;
	GameAction* action = new GameAction(key_code, behaviour, name);
	if(keys.size() == 0) keys.reserve(5);
	std::cout << "Size : " << keys.size() << std::endl;
	keys.push_back(action);
	std::cout << "Size : " << keys.size() << std::endl;
	return action;
}

GameAction* GameAction::add_mouse_bind(const int mouse_code, const BEHAVIOURS behaviour, const std::string& name) {
	if(mouse_code < 0 || mouse_code > 8) { fprintf(stderr, "The mouse code provided %d belonging to key %s is incorrect. Please select from the approved mouse codes.\n", mouse_code, glfwGetKeyName(mouse_code, 0)); return nullptr; }
	else {
		std::cout << "Adding mouse bind" << std::endl;
		GameAction* mouse = new GameAction(mouse_code, behaviour, name);
		mouses[mouse_code] = mouse;
		return mouse;
	}
}

int GameAction::get_amount() {
	if(amount == 0) return 0;
	int amount = this->amount;
	this->amount = 0;
	return amount;
}

bool GameAction::is_pressed() { return get_amount() != 0; }

void GameAction::consume() { amount = 0; }

void GameAction::dispose_input_controls() {
	for(unsigned int i = 0; i < keys.size(); i++) delete keys[i];
	keys.clear();
	for(unsigned int i = 0; i < 9; i++) if(mouses[i]) delete mouses[i];
}

void GameAction::press(const int& amount) {
	if(state != GAME_INPUT_STATE_WAITING_TO_BE_RELEASED) {
		this->amount += amount;
		state = GAME_INPUT_STATE_PRESSED;
	}
}

void GameAction::release() { state = GAME_INPUT_STATE_RELEASED; }

void GameAction::_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	for(unsigned int i = 0; i < keys.size(); i++) {
		GameAction* game_action = keys[i];
		if(game_action == nullptr) return;
		if(game_action->key == key) {
			if(action != GLFW_RELEASE) game_action->press(1);
			else game_action->release();
		}
	}
}

void GameAction::mouse_position_helper(const unsigned int code_neg, const unsigned int code_pos, const double amount) {
	if(amount != 0) {
		GameAction* action = mouses[amount < 0 ? code_neg : code_pos];
		if(action != NULL) { action->press(amount > 0 ? amount : -amount); action->release(); }
	}
}

void GameAction::_mouse_position_callback(GLFWwindow* window, double x, double y) {
	double diff_x = x - Core::mouseX, diff_y = y - Core::mouseX;
	mouse_position_helper(MM_LEFT, MM_RIGHT, diff_x);
	mouse_position_helper(MM_DOWN, MM_UP, diff_y);
	Core::_set_mouse_coords(x, y);
}

void GameAction::_mouse_scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	GameAction *up = mouses[MW_UP], *down = mouses[MW_DOWN];
	if(yoffset > 0) { if(up) up->press(yoffset); }
	else if(yoffset < 0) { if(down) down->press(-yoffset); }
	else { if(up) up->release(); if(down) down->release(); }
}

void GameAction::_mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
	GameAction* game_action = mouses[button];
	if(game_action) {
		if(action != GLFW_RELEASE) game_action->press(1);
		else game_action->release();
	}
}

std::ostream& Cyclone::operator<<(std::ostream& stream, const GameAction& action) {
	stream << "Game Action : " << action.name << ", Key : " <<
		/*glfwGetKeyName(action.key, glfwGetKeyScancode(action.key))*/ action.key << ", State : " <<
		(action.state == GAME_INPUT_STATE_PRESSED ? "Pressed" : (action.state == GAME_INPUT_STATE_WAITING_TO_BE_RELEASED ? "Waiting to be released..." : "Released")) << ", Behaviour : " <<
		(action.behaviour == GAME_INPUT_BEHAVIOUR_NORMAL ? "Normal" : "Detect Initial Press Only.");
	return stream;
}

void GameAction::dispose() { delete this; }
