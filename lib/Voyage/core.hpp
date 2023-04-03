#pragma once
#include <unordered_map>
#include <voyage.hpp>
#include "Voyage/game_action.hpp"

namespace Voyage {

	class Core {
		public:
			static double startTime, endTime, deltaTime, mouseX, mouseY;
			static int width, height;

			Core() noexcept = default;

			~Core() noexcept = default;

			static void getScreenResolution(int& width, int& height);

			static void _set_mouse_coords(const double, const double);
		protected:
			static GLFWwindow* window;
			static bool is_fullscreen, is_vsync, is_mouse_locked, is_mouse_moving;

			static void init();

			static void set_fullscreen(const bool& fullscreen);

			static void set_vsync(const int& vsync);

			static void set_screen_resolution(const uint32_t a_width, const uint32_t a_height);

			static uint32_t getWidth();

			static uint32_t getHeight();

			static void set_background_color(const float& red, const float& green, const float& blue, const float& alpha = 1.0);

			virtual void update(const double& deltaTime) = 0;

			virtual void dispose() = 0;

			void startGame();

			static void setCursorDisabled(const bool& disabled);

			static void hide_and_lock_cursor(const bool& cursor);

			static void stopGame();
	};

}
