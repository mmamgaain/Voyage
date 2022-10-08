#pragma once
#include <voyage.hpp>
#include "extra/input/game_action.hpp"

namespace Voyage {

	class Core {
		public:
			static bool is_debug;
			static double startTime, endTime, deltaTime, mouseX, mouseY;
			static size_t width, height;

			Core() noexcept = default;

			~Core() noexcept = default;

			static void _error_callback(const int, const char*);

			static void _set_mouse_coords(const double, const double);

		protected:
			static GLFWwindow* window;
			static bool is_fullscreen, is_vsync, is_mouse_locked, is_mouse_moving;

			static void init();

			static void set_fullscreen(const bool& fullscreen);

			static void set_vsync(const int& vsync);

			static void set_screen_resolution(const unsigned int a_width, const unsigned int a_height);

			static unsigned int getWidth();

			static unsigned int getHeight();

			static void set_background_color(const float& red, const float& green, const float& blue, const float& alpha = 1.0);

			virtual void update(double deltaTime) = 0;

			virtual void dispose() = 0;

			void startGame();

			static void setCursorDisabled(const bool& disabled);

			static void hide_and_lock_cursor(const bool& cursor);

			static void stopGame();
	};

}
