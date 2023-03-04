# Voyage Rendering Engine

## Introduction
This is a **hobby project** that I've started in my spare time. This engine is being written in C++.
As this is a personal project, the scope of this project is very limited. **This project is only available on Linux as I work with Linux.**

This project contains libraries written by people more talented than I am. These libraries are:
- [GLM](https://github.com/g-truc/glm "OpenGL Mathematics (GLM)") - Standard Maths library.
- [GLFW](https://raw.githubusercontent.com/glfw/glfw "GLFW") - Library for window management and keyboard and mouse input handling.
- [glad](https://github.com/Dav1dde/glad "glad") - Modern OpenGL library for C++.
- [stb](https://github.com/nothings/stb "stb") - Image loading.
- [ImGui](https://github.com/ocornut/imgui "Dear ImGui") - Immediate Mode GUI; absolutely amazing.
- [Assimp](https://github.com/assimp/assimp "Open Asset Import Library (assimp)") - Model loading.
- [OpenAL](https://github.com/kcat/openal-soft "OpenAL soft") - GPU accelerated sounds.
- [libsndfile](https://github.com/libsndfile/libsndfile "libsndfile") - Loading sound files (except MP3 files).
- [minimp3](https://github.com/lieff/minimp3 "minimp3") - Loading MP3 files.

## Requirements
- g++
- Makefile

## Basic Build Instructions (for running tests)
To run the only test in the project currently, simply navigate to the project directory and type `make` in the command line. This will build your entire project and also execute it. `make fresh` will remove all the intermediate object files and the executable.

`make compile_executable` and `make compile_debug_executable` will compile release and debug executables, respectively.

## Build Instructions
There are two main ways to build and work with this project:
### 1. Building as an external library:
`make compile_as_static_library` and `make compile_as_dynamic_library` are commands to compile the project as a static and dynamic library, respectively. The static library output "libvoyage.a" and the dynamic library output "libvoyage.so" are produced in the base directory. All the header files are available in `lib/` directory and all the binary files for external libraries are in the `build/precompiled/` directory.
### 2. Building with project skeleton (Recommended):
To execute this, issue command `make export`. This will create a directory with a default name of "Voyage_project" at the current location of this project. This project will contain all the directories and files already created. The only reason that this method is recommended is that it creates the whole structure and even creates a stock main file.

## Project Overview
To start using the project, we'll have to include the project header files. We can either include individual header files or we can include the file `lib/Voyage.hpp` which is a single header file for the whole project.

If you've run `make export` to create the entire project skeleton, you'll find a stock main file at `src/main.cpp` looking something like:
``` c++
// Welcome to a new Voyage Project.
#include "Voyage/core.hpp"
// #include "Voyage.hpp"

using namespace Voyage;

class Main : public Core {
	public:
		Main() {}

		void update(double deltaTime) override {}

		void dispose() override {}

	private:
};

int main() {
	Main main_instance;
	return 0;
}
```
The update method is the part of the game loop that's exposed to the developer. All of the game logic goes in here. To initialise a basic game loop, you'll probably want a constructor of the Main class like this:
```c++
Main() {
	Core::set_fullscreen(true); 		 // Sets the window as a fullscreen window. This call has to be before call to init().
										 // All calls after init() are ignored. DEFAULT: false
	Core::set_vsync(0); 				 // Sets the VSync OFF. This call has to be before call to init().
										 // All calls after init() are ignored. DEFAULT: 1
	Core::init(); 						 // init() initialises GLFW and OpenGL. It creates Event
										 // hooks and gets them listening to event calls
	Core::set_background_color(1, 1, 1); // Sets the Background Color to White(1, 1, 1). Cannot be called before init()
	Core::hide_and_lock_cursor(true); 	 // Hides the visible mouse cursor icon and locks it in place in the center of the screen.
									  	 // This will be more useful when operating with a 3D Camera.

	Core::startGame(); 					 // The function that kickstarts the game-loop.
										 // It should be called at the end of the constructor.
}
```

Be careful with a fullscreen application as the close button to exit application will not be visible. To help here we'll need keyboard and mouse events.

### KEYBOARD AND MOUSE EVENTS:
The class named `GameAction` represents the keyboard and mouse action instance. To create a simple keyboard key to exit the application:
```c++
// Welcome to a new Voyage Project.
#include <Voyage/core.hpp>
#include <Voyage/game_action.hpp>

class Main : public Core {
	public:
		Main() {
			// ...
			exit = GameAction::add_key_bind(GLFW_KEY_ESCAPE); // Methods like add_key_bind, add_mouse_bind allow
															  // the creation of user initiated events

			Core::startGame();
		}

		void update(double deltaTime) override {
			if(exit->isPressed()) stopGame(); // We can access the method isPressed or getAmount to check if the event has been triggered
											  // since last checked
		}

		void dispose() override {}

	private:
		GameAction *exit; // Declare a GameAction pointer
}

int main() {
	Main main_instance;
	return 0;
}
```

The methods for adding key and mouse listener are:
```c++
// key_code - The keycode is any GLFW keycode for keyboard
// behaviour - GAME_INPUT_BEHAVIOUR_NORMAL (default), GAME_INPUT_BEHAVIOUR_DETECT_INITIAL_PRESS_ONLY (Continuously pressing this key will only register this key having been pressed once)
// name - Debug feature. Completely optional
GameAction* add_key_bind(const int& key_code, const BEHAVIOURS behaviour = GAME_INPUT_BEHAVIOUR_NORMAL, const char* const name = "Unknown");

// mouse_code - The mouse keycodes are integer constants with prefixes for different type of mouse actions.
// 1. MB_ prefix - Mouse Button actions. Ex, MB_LEFT, MB_RIGHT, MB_MIDDLE.
// 2. MM_ prefix - Mouse Movement actions. Ex, MM_LEFT, MM_RIGHT, MM_UP, MM_DOWN.
// 3. MW_ prefix - Mouse Wheel actions. Ex, MW_UP, MW_DOWN.
// behaviour - GAME_INPUT_BEHAVIOUR_NORMAL (default), GAME_INPUT_BEHAVIOUR_DETECT_INITIAL_PRESS_ONLY (Continuously pressing this key will only register this key having been pressed once)
// name - Debug feature. Completely optional
GameAction* add_mouse_bind(const int& mouse_code, const BEHAVIOURS behaviour = GAME_INPUT_BEHAVIOUR_NORMAL, const char* const name = "Unknown");
```
