# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.30

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /snap/cmake/1409/bin/cmake

# The command to remove a file.
RM = /snap/cmake/1409/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/walid/Downloads/temp_workspace/sumo_robot/bots2d

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/walid/Downloads/temp_workspace/sumo_robot/bots2d/nsumoapp

# Utility rule file for docs.

# Include any custom commands dependencies for this target.
include external/glfw/docs/CMakeFiles/docs.dir/compiler_depend.make

# Include the progress variables for this target.
include external/glfw/docs/CMakeFiles/docs.dir/progress.make

external/glfw/docs/CMakeFiles/docs:
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --blue --bold --progress-dir=/home/walid/Downloads/temp_workspace/sumo_robot/bots2d/nsumoapp/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Generating HTML documentation"
	cd /home/walid/Downloads/temp_workspace/sumo_robot/bots2d/nsumoapp/external/glfw/docs && /usr/bin/doxygen

docs: external/glfw/docs/CMakeFiles/docs
docs: external/glfw/docs/CMakeFiles/docs.dir/build.make
.PHONY : docs

# Rule to build all files generated by this target.
external/glfw/docs/CMakeFiles/docs.dir/build: docs
.PHONY : external/glfw/docs/CMakeFiles/docs.dir/build

external/glfw/docs/CMakeFiles/docs.dir/clean:
	cd /home/walid/Downloads/temp_workspace/sumo_robot/bots2d/nsumoapp/external/glfw/docs && $(CMAKE_COMMAND) -P CMakeFiles/docs.dir/cmake_clean.cmake
.PHONY : external/glfw/docs/CMakeFiles/docs.dir/clean

external/glfw/docs/CMakeFiles/docs.dir/depend:
	cd /home/walid/Downloads/temp_workspace/sumo_robot/bots2d/nsumoapp && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/walid/Downloads/temp_workspace/sumo_robot/bots2d /home/walid/Downloads/temp_workspace/sumo_robot/bots2d/external/glfw/docs /home/walid/Downloads/temp_workspace/sumo_robot/bots2d/nsumoapp /home/walid/Downloads/temp_workspace/sumo_robot/bots2d/nsumoapp/external/glfw/docs /home/walid/Downloads/temp_workspace/sumo_robot/bots2d/nsumoapp/external/glfw/docs/CMakeFiles/docs.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : external/glfw/docs/CMakeFiles/docs.dir/depend

