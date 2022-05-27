#pragma once
#include "../../lib/glfw3.h"
#include <ostream>
#include <vector>
#include <string>
#include <iostream>

#define DEFAULT_INITIAL_DARRAY_COLLECTION_SIZE 5
#define DEFAULT_MINIMUM_STRING_REALLOCATION_SIZE 50
#define DEFAULT_HASHMAP_SIZE 50
/* #define MB_RIGHT GLFW_MOUSE_BUTTON_RIGHT
#define MB_MIDDLE GLFW_MOUSE_BUTTON_MIDDLE
#define MB_LEFT GLFW_MOUSE_BUTTON_LEFT */
#define MB_LEFT 0
#define MB_RIGHT 1
#define MB_MIDDLE 2
#define MW_UP 3
#define MW_DOWN 4
#define MM_UP 5
#define MM_DOWN 6
#define MM_LEFT 7
#define MM_RIGHT 8

#define NO_TEXTURE 0

#define SIZE_OF_ARRAY_IN_ELEMENTS(x) (sizeof(x) / sizeof(x[0]))
#define SIZE_OF_ARRAY_IN_BYTES(x, length) (sizeof(x) / length)

namespace Cyclone {

	typedef enum { GAME_INPUT_BEHAVIOUR_NORMAL, GAME_INPUT_BEHAVIOUR_DETECT_INITIAL_PRESS_ONLY } BEHAVIOURS;
	typedef enum { GAME_INPUT_STATE_RELEASED, GAME_INPUT_STATE_PRESSED, GAME_INPUT_STATE_WAITING_TO_BE_RELEASED } STATES;

	class GameAction {
		private:
			unsigned int amount, key;
			std::string name;
			BEHAVIOURS behaviour;
			STATES state;

			static std::vector<GameAction*> keys;
			static GameAction* mouses[9];

			GameAction(const int key_code, const BEHAVIOURS behaviour, const std::string& name);

			GameAction(const GameAction& game_action);

			void press(const int& amount);

			void release();

			static void mouse_position_helper(const unsigned int, const unsigned int, const double);
		public:
			GameAction(GameAction&& game_action);

			~GameAction();

			static GameAction* add_key_bind(const int key_code, const BEHAVIOURS behaviour = GAME_INPUT_BEHAVIOUR_NORMAL, const std::string& name = "Unknown");

			static GameAction* add_mouse_bind(const int mouse_code, const BEHAVIOURS behaviour = GAME_INPUT_BEHAVIOUR_NORMAL, const std::string& name = "Unknown");

			int get_amount();

			bool is_pressed();

			void consume();

			static void dispose_input_controls();

			static void _key_callback(GLFWwindow*, int, int, int, int);

			static void _mouse_position_callback(GLFWwindow* window, double x, double y);

			static void _mouse_scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

			static void _mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

			void dispose();

			friend std::ostream& operator<<(std::ostream& stream, const GameAction& action);
	};

	std::ostream& operator<<(std::ostream& stream, const GameAction& action);

}

