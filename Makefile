# Make file for C++ OpenGL first project
#
# AUTHOR:- MAYANK MAMGAAIN
#
# TEMPLATE:
# target: dependencies
# 	action

# DECLARING GLOBAL VARIABLES STARTED ##
o_dir := build/
o_precompiled_dir := $(o_dir)precompiled/
lib_dir := lib/
src_dir := src/
final_switches = -Wall -lglfw -ldl -lm -lassimp#-lGL -lXrandr -lXxf86vm -lXi -pthread -lXinerama -lX11 -lrt
object_switches = -c
src_files := $(shell find $(src_dir) -type f -name "*.cpp")
header_files := $(subst src/, ,$(shell find $(src_dir) -type f -name "*.hpp"))
o_files := $(src_files:.cpp=.o)
o_files_debug := $(src_files:.cpp=.o)
export_proj_name := ../cy_project/
# DECLARING GLOBAL VARIABLES ENDED #####

# RULES DEFINITION STARTED ############
# Default rule
main: compile_executable
	./main && rm main

# Rule to compile and export the project to be incorporated into other projects
export: compile_debug_objects
	mkdir $(export_proj_name) $(export_proj_name)src/ && touch $(export_proj_name)src/main.cpp; cp -t $(export_proj_name) -r build/ lib/ res/ Makefile && cp -t $(export_proj_name)lib --parents $(addprefix src/, $(header_files)) && mv -t $(export_proj_name)$(o_precompiled_dir) $(export_proj_name)$(o_dir)*.o && make clean

# Pattern rule to just compile the source files and create an executable in the current directory
compile_executable: compile_objects
	g++ -o main $(o_dir)*.o $(o_precompiled_dir)* $(final_switches) && make clean

compile_debug_executable: compile_debug_objects
	g++ -o main $(o_dir)*.o $(o_precompiled_dir)* $(final_switches) && make clean

# Pattern rule to just compile the source files to object files in the build directory
compile_objects: $(o_files)

# Pattern rule to just compile the source files to object files in the build directory
compile_debug_objects: debug

debug: final_switches += -g
debug: object_switches += -g
debug: $(o_files)

# Pattern rule to match all the object files
$(o_files): $(src_files)
	g++ $(object_switches) $*.cpp -o $(o_dir)$(notdir $@)

# Rule to clean the intermediate object(*.o) files
clean:
	rm $(o_dir)*.o

# Create a fresh start, i.e., clean and remove the executable
fresh:
	make clean && rm main
# RULES DEFINITION ENDED  #############
