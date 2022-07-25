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
o_precompiled_files := $(shell find $(o_precompiled_dir) -type f -name "*")
lib_dir := lib/
src_dir := src/
final_switches := -Wall -pthread -ldl -lm -std=c++17#-lGL -lXrandr -lXxf86vm -lXi -lXinerama -lX11 -lrt
object_switches := -c -O3
debug_object_switches := $(object_switches) -g
src_files := $(shell find $(src_dir) -type f -name "*.cpp")
header_files := $(subst src/, ,$(shell find $(src_dir) -type f -name "*.hpp"))
pch_source := $(lib_dir)voyage.hpp
pch_out := $(lib_dir)voyage.hpp.gch
o_files := $(src_files:.cpp=.o)
debug_o_files := $(src_files:.cpp=_d.o)
export_proj_name := ../Voyage_project/
export_main_placeholder := "// Welcome to a new Voyage Project.\n\#include <voyage.hpp>\n\#include <voyage/extra/core/core.hpp>\n\nclass Main : public Voyage::Core {\n\tpublic:\n\t\tvoid update(double deltaTime) override {}\n\n\t\tvoid dispose() override {}\n\n\tprivate\n}\n\nint main() {\n\tMain main_instance;\n\treturn 0;\n}\n"
# DECLARING GLOBAL VARIABLES ENDED #####

# RULES DEFINITION STARTED ############
# Default rule
main: compile_executable
	@echo "Executing '$(abspath main)' application..." && ./main

# Rule to compile and export the project to be incorporated into other projects
export: compile_debug_objects
	@mkdir $(export_proj_name) $(export_proj_name)src/ && echo "Creating New Project '$(abspath $(export_proj_name))...'" && echo $(export_main_placeholder) > $(export_proj_name)src/main.cpp && echo "Writing a biolerplate main file '$(abspath $(export_proj_name))/src/main.cpp'"; cp -t $(export_proj_name) -r $(o_dir) $(lib_dir) res/ Makefile .vimspector.json .ycm_extra_conf.py && mkdir $(export_proj_name)$(o_dir)precompiled/voyage/ $(export_proj_name)$(lib_dir)voyage/ && mv -t $(export_proj_name)$(o_dir)precompiled/voyage/ $(export_proj_name)$(o_dir)*.o && cp -t $(export_proj_name)$(lib_dir) --parents $(addprefix src/, $(header_files)) && echo "Creating and moving important assets from main project to export project"

# Pattern rule to just compile the source files and create an executable in the current directory
compile_executable: compile_objects
	@$(cc) -o main $(addprefix $(o_dir), $(notdir $(o_files))) $(o_precompiled_files) $(final_switches) && echo "Compiling and linking into executable '$(abspath main)'"

compile_debug_executable: compile_debug_objects
	@$(cc) -o main_d $(addprefix $(o_dir), $(notdir $(debug_o_files))) $(o_precompiled_files) $(final_switches) && echo "Compiling and linking into debug executable '$(abspath main_d)'"

# Pattern rule to just compile the source files to object files in the build directory
compile_objects: $(pch_out) $(o_files)

# Pattern rule to just compile the source files to object files in the build directory
compile_debug_objects: $(pch_out) $(debug_o_files)

$(pch_out): $(pch_source)
	@$(cc) $(final_switches) -g -I$(lib_dir) -Xlinker --verbose -shared -o $@ $^ && echo "Voyage Precompiled header was found to be outdated.\nCompiling Voyage PCH to $(abspath $(pch_out))"

# Pattern rule to match all the object files
$(o_files): $(src_files)
	@if ! [ -f $(o_dir)$(notdir $@) ] || [ `date -r $(o_dir)$(notdir $@) +%s` -lt `date -r $*.cpp +%s` ]; then $(cc) $(object_switches) -I$(lib_dir) $*.cpp -o $(o_dir)$(notdir $@) && echo "Compiling $(lib_dir)$(notdir $@) from $(basename $@).cpp..."; else echo "Object file $@ already up-to-date."; fi

# Pattern rule to match all the debug object files
$(debug_o_files): $(src_files)
	@if ! [ -f $(o_dir)$(notdir $@) ] || [ `date -r $(o_dir)$(notdir $@) +%s` -lt `date -r $(*:_d=.cpp) +%s` ]; then $(cc) $(object_switches) -I$(lib_dir) $(*:_d=.cpp) -o $(o_dir)$(notdir $@) && echo "Compiling $(lib_dir)$(notdir $@) from $(patsubst %_d,%.cpp, $(basename $@))..."; else echo "Object file $@ already up-to-date."; fi

# Rule to clean the intermediate object(*.o) files
clean:
	@if [ "$(shell ls $(o_dir)*.o 2> /dev/null)" != "" ]; then rm $(o_dir)*.o && echo "Removing all compiled object files"; fi

# Create a fresh start, i.e., clean and remove the executable
fresh:
	@make clean; if [ -f main ]; then rm main; has_ex=1; fi; if [ -f main_d ]; then rm main_d; has_ex=1; fi; echo "Disposing the previously generated executable."
# RULES DEFINITION ENDED  #############
