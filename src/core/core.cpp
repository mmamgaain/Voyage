#include "../../lib/glad.h"
#include "core.hpp"
#include "../../lib/glfw3.h"
#include "../input/game_action.hpp"
#include <stdio.h>
#include <stdlib.h>

using namespace Cyclone;

bool Core::is_debug = false;
double Core::startTime = 0, Core::endTime = 0, Core::deltaTime = 0, Core::mouseX = 0, Core::mouseY = 0;
GLFWwindow* Core::window;
size_t Core::width, Core::height;
bool Core::is_fullscreen = false, Core::is_vsync = true, Core::is_mouse_locked = false, Core::is_mouse_moving = false;

Core::Core() {}

Core::~Core() {}

void Core::_error_callback(const int error, const char* description) { fputs(description, stderr); }

void Core::_set_mouse_coords(const double x, const double y) { mouseX = x; mouseY = y; }

void Core::init() {
	if(!glfwInit()) { fprintf(stderr, "ERROR: GLFW could not be initialized\n"); exit(EXIT_FAILURE); }
	glfwDefaultWindowHints();
	glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
	glfwWindowHint(GLFW_RESIZABLE, is_fullscreen ? GLFW_FALSE : GLFW_TRUE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwSetErrorCallback(_error_callback);
	window = glfwCreateWindow(width, height, "OpenGL Application", is_fullscreen ? glfwGetPrimaryMonitor() : NULL, NULL);
	if(!window) { fprintf(stderr, "ERROR: GLFW Window could not be initialized\n"); glfwTerminate(); exit(EXIT_FAILURE); }
	glfwSetKeyCallback(window, GameAction::_key_callback);
	glfwSetCursorPosCallback(window, GameAction::_mouse_position_callback);
	glfwSetMouseButtonCallback(window, GameAction::_mouse_button_callback);
	glfwSetScrollCallback(window, GameAction::_mouse_scroll_callback);
	glfwMakeContextCurrent(window);
	glfwSwapInterval(is_vsync ? 1 : 0);
	if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) { fprintf(stderr, "ERROR: Modern OpenGL from GLAD could not be loaded\n"); glfwTerminate(); exit(EXIT_FAILURE);  }
	if(!is_debug) glfwShowWindow(window);
	glViewport(0, 0, width, height);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
}

void Core::set_fullscreen(const bool& fullscreen) { is_fullscreen = fullscreen; }

void Core::set_vsync(const int& vsync) { is_vsync = vsync; }

void Core::set_screen_resolution(const unsigned int a_width, const unsigned int a_height) { width = a_width; height = a_height; }

unsigned int Core::getWidth() { return width; }

unsigned int Core::getHeight() { return height; }

void Core::set_background_color(const float& red, const float& green, const float& blue, const float& alpha) { glClearColor(red, green, blue, alpha); }

void Core::startGame() {
	startTime = glfwGetTime();
	while(!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		// start frame
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		update(deltaTime);

		glfwSwapBuffers(window);
		endTime = glfwGetTime();
		deltaTime = endTime - startTime;
		startTime = endTime;
	}
	dispose();
	GameAction::dispose_input_controls();
	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}

void Core::setCursorDisabled(const bool& disabled) { glfwSetInputMode(window, GLFW_CURSOR, disabled ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL); }

void Core::hide_and_lock_cursor(const bool& cursor) {
	is_mouse_locked = cursor;
	mouseX = width / 2.0;
	mouseY = height / 2.0;
	glfwSetCursorPos(window, mouseX, mouseY);
	setCursorDisabled(cursor);
}

void Core::stopGame() { glfwSetWindowShouldClose(window, GLFW_TRUE); }

