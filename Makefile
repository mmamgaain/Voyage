# Make file for C++ OpenGL first project
#
# AUTHOR:- MAYANK MAMGAAIN
#
# TEMPLATE:
# target: dependencies
# 	action

# DECLARING GLOBAL VARIABLES STARTED ##
cc := g++
o_dir := build/
o_precompiled_dir := $(o_dir)precompiled/
o_precompiled_files := $(shell find $(o_precompiled_dir) -type f)
lib_dir := lib/
src_dir := src/
final_switches := -Wall -pthread -ldl -lm -xc++ --std=c++17 -O3#-lGL -lXrandr -lXxf86vm -lXi -lXinerama -lX11 -lrt
object_switches := -c -O3 -xc++ --std=c++17
debug_object_switches := $(object_switches) -g
src_files := $(shell find $(src_dir) -type f -name "*.cpp")
header_files := $(subst src/, ,$(shell find $(src_dir) -type f -name "*.hpp"))
pch_source := $(lib_dir)voyage.hpp
pch_out := $(lib_dir)voyage.hpp.gch
o_files := $(src_files:.cpp=.o)
debug_o_files := $(src_files:.cpp=_d.o)
export_proj_name := ../Voyage_project/
export_mainfile_placeholder := "// Welcome to a new Voyage Project.\n\#include <voyage.hpp>\n\#include <voyage/extra/core/core.hpp>\n\nclass Main : public Voyage::Core {\n\tpublic:\n\t\tvoid update(double deltaTime) override {}\n\n\t\tvoid dispose() override {}\n\n\tprivate\n}\n\nint main() {\n\tMain main_instance;\n\treturn 0;\n}\n"
# executable_outdated := 0
# DECLARING GLOBAL VARIABLES ENDED #####

# RULES DEFINITION STARTED ############
# Special .PHONY rule; this rule cannot really be referenced
.PHONY: run_release run_debug export compile_release_executable compile_debug_executable compile_objects compile_debug_objects clean fresh

# Default rule; to run the release executable
run_release: compile_release_executable
	@echo "Executing '$(abspath main)' application..." && ./main

# Rule to execute the debug executable
run_debug: compile_debug_executable
	@echo "Executing '$(abspath main_d)' application..." && ./main_d

# Rule to compile and export the project to be incorporated into other projects
export: compile_debug_objects
	@mkdir $(export_proj_name) $(export_proj_name)src/ && echo "Creating New Project '$(abspath $(export_proj_name))...'" && echo $(export_mainfile_placeholder) > $(export_proj_name)src/main.cpp && echo "Writing a biolerplate main file '$(abspath $(export_proj_name))/src/main.cpp'"; cp -t $(export_proj_name) -r $(o_dir) $(lib_dir) res/ Makefile .vimspector.json .ycm_extra_conf.py && mkdir $(export_proj_name)$(o_dir)precompiled/voyage/ $(export_proj_name)$(lib_dir)voyage/ && mv -t $(export_proj_name)$(o_dir)precompiled/voyage/ $(export_proj_name)$(o_dir)*.o && cp -t $(export_proj_name)$(lib_dir) --parents $(addprefix src/, $(header_files)) && echo "Creating and moving important assets from main project to export project"

# Pattern rule to just compile the source files and create an executable in the current directory
compile_release_executable: compile_objects
	@echo "Compiling and linking into executable '$(abspath main)'" && $(cc) -o main $(addprefix $(o_dir), $(notdir $(o_files))) $(o_precompiled_files) $(final_switches)
	@#; else echo "Compiled executable '$(abspath main)' already found up-to-date"

compile_debug_executable: compile_debug_objects
	@echo "Compiling and linking into debug executable '$(abspath main_d)'" && $(cc) -o main_d $(addprefix $(o_dir), $(notdir $(debug_o_files))) $(o_precompiled_files) $(final_switches)

# Pattern rule to just compile the source files to object files in the build directory
compile_objects: $(pch_out) $(o_files)

# Pattern rule to just compile the source files to object files in the build directory
compile_debug_objects: $(pch_out) $(debug_o_files)

$(pch_out): $(pch_source)
	@echo "Voyage Precompiled header was found to be outdated.\nCompiling Voyage PCH to $(abspath $(pch_out))" && $(cc) $(debug_object_switches) -I$(src_dir) -I$(lib_dir) -Xlinker --verbose -shared -o $@ $^

# Pattern rule to match all the object files
$(o_files): $(src_files)
	@if ! [ -f $(o_dir)$(notdir $@) ] || [ `date -r $(o_dir)$(notdir $@) +%s` -lt `date -r $*.cpp +%s` ]; then $(cc) $(object_switches) -I$(src_dir) -I$(lib_dir) -I$(lib_dir)PhysX/physx/ -I$(lib_dir)PhysX/pxshared/ $*.cpp -o $(o_dir)$(notdir $@) && echo "Compiling $(lib_dir)$(notdir $@) from $(basename $@).cpp..."; else echo "Object file $(lib_dir)$(notdir $@) already up-to-date."; fi

# Pattern rule to match all the debug object files
$(debug_o_files): $(src_files)
	@if ! [ -f $(o_dir)$(notdir $@) ] || [ `date -r $(o_dir)$(notdir $@) +%s` -lt `date -r $(*:_d=.cpp) +%s` ]; then $(cc) $(debug_object_switches) -I$(src_dir) -I$(lib_dir) -I$(lib_dir)PhysX/physx/ -I$(lib_dir)PhysX/pxshared/ $(*:_d=.cpp) -o $(o_dir)$(notdir $@) && echo "Compiling $(lib_dir)$(notdir $@) from $(patsubst %_d,%.cpp, $(basename $@))..."; else echo "Object file $@ already up-to-date."; fi

# Rule to clean the intermediate object(*.o) files
clean:
	@if [ "$(shell ls $(o_dir)*.o 2> /dev/null)" != "" ]; then rm $(o_dir)*.o; echo "Removing all compiled object files"; fi

# Create a fresh start, i.e., clean and remove the executable
fresh: clean
	@if [ -f main ]; then rm main; export has_ex=1; fi; if [ -f main_d ]; then rm main_d; export has_ex=1; fi; [ -z $has_ex ] && echo "Disposing the previously generated executable."; export has_ex=;
# RULES DEFINITION ENDED  #############
