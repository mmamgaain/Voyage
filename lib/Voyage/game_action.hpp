#pragma once
#include <voyage.hpp>

/* #define DEFAULT_INITIAL_DARRAY_COLLECTION_SIZE 5
#define DEFAULT_MINIMUM_STRING_REALLOCATION_SIZE 50
#define DEFAULT_HASHMAP_SIZE 50 */
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

namespace Voyage {

	typedef enum { GAME_INPUT_BEHAVIOUR_NORMAL, GAME_INPUT_BEHAVIOUR_DETECT_INITIAL_PRESS_ONLY } BEHAVIOURS;
	typedef enum { GAME_INPUT_STATE_RELEASED, GAME_INPUT_STATE_PRESSED, GAME_INPUT_STATE_WAITING_FOR_RELEASE } STATES;

	class GameAction {
		public:
			static GameAction* add_key_bind(const int& key_code, const BEHAVIOURS behaviour = GAME_INPUT_BEHAVIOUR_NORMAL, const char* const name = "Unknown");

			static GameAction* add_mouse_bind(const int& mouse_code, const BEHAVIOURS behaviour = GAME_INPUT_BEHAVIOUR_NORMAL, const char* const name = "Unknown");

			GameAction& operator=(const GameAction& lhs) = delete;

			GameAction& operator=(GameAction&& rhs) = delete;

			~GameAction() noexcept;

			float getAmount();

			bool isPressed();

			void consume();

			void reset();

			static void dispose_input_controls();

			static void _key_callback(GLFWwindow*, int, int, int, int);

			static void _mouse_position_callback(GLFWwindow*, double, double);

			static void _mouse_scroll_callback(GLFWwindow*, double, double);

			static void _mouse_button_callback(GLFWwindow*, int, int, int);

			void dispose();

			friend std::ostream& operator<<(std::ostream& stream, const GameAction& action);
		private:
			uint32_t amount, key;
			std::string name;
			BEHAVIOURS behaviour;
			STATES state;
			static std::unordered_map<int, GameAction*> keys;
			static GameAction* mouses[9];

			GameAction(const int, const BEHAVIOURS, const std::string&);

			GameAction(const GameAction&);

			GameAction(GameAction&&);

			void press(const int& = 1);

			void tap(const int& = 1);

			void release();

			static void mouse_position_helper(const uint32_t, const uint32_t, const double);
	};

	std::ostream& operator<<(std::ostream& stream, const GameAction& action);
}

