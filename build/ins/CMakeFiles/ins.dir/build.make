# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list

# Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/zmm/pl0compiler

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/zmm/pl0compiler/build

# Include any dependencies generated for this target.
include ins/CMakeFiles/ins.dir/depend.make

# Include the progress variables for this target.
include ins/CMakeFiles/ins.dir/progress.make

# Include the compile flags for this target's objects.
include ins/CMakeFiles/ins.dir/flags.make

ins/CMakeFiles/ins.dir/ins.c.o: ins/CMakeFiles/ins.dir/flags.make
ins/CMakeFiles/ins.dir/ins.c.o: ../ins/ins.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/zmm/pl0compiler/build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object ins/CMakeFiles/ins.dir/ins.c.o"
	cd /home/zmm/pl0compiler/build/ins && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/ins.dir/ins.c.o   -c /home/zmm/pl0compiler/ins/ins.c

ins/CMakeFiles/ins.dir/ins.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/ins.dir/ins.c.i"
	cd /home/zmm/pl0compiler/build/ins && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /home/zmm/pl0compiler/ins/ins.c > CMakeFiles/ins.dir/ins.c.i

ins/CMakeFiles/ins.dir/ins.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/ins.dir/ins.c.s"
	cd /home/zmm/pl0compiler/build/ins && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /home/zmm/pl0compiler/ins/ins.c -o CMakeFiles/ins.dir/ins.c.s

ins/CMakeFiles/ins.dir/ins.c.o.requires:
.PHONY : ins/CMakeFiles/ins.dir/ins.c.o.requires

ins/CMakeFiles/ins.dir/ins.c.o.provides: ins/CMakeFiles/ins.dir/ins.c.o.requires
	$(MAKE) -f ins/CMakeFiles/ins.dir/build.make ins/CMakeFiles/ins.dir/ins.c.o.provides.build
.PHONY : ins/CMakeFiles/ins.dir/ins.c.o.provides

ins/CMakeFiles/ins.dir/ins.c.o.provides.build: ins/CMakeFiles/ins.dir/ins.c.o

# Object files for target ins
ins_OBJECTS = \
"CMakeFiles/ins.dir/ins.c.o"

# External object files for target ins
ins_EXTERNAL_OBJECTS =

ins/libins.a: ins/CMakeFiles/ins.dir/ins.c.o
ins/libins.a: ins/CMakeFiles/ins.dir/build.make
ins/libins.a: ins/CMakeFiles/ins.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking C static library libins.a"
	cd /home/zmm/pl0compiler/build/ins && $(CMAKE_COMMAND) -P CMakeFiles/ins.dir/cmake_clean_target.cmake
	cd /home/zmm/pl0compiler/build/ins && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/ins.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
ins/CMakeFiles/ins.dir/build: ins/libins.a
.PHONY : ins/CMakeFiles/ins.dir/build

ins/CMakeFiles/ins.dir/requires: ins/CMakeFiles/ins.dir/ins.c.o.requires
.PHONY : ins/CMakeFiles/ins.dir/requires

ins/CMakeFiles/ins.dir/clean:
	cd /home/zmm/pl0compiler/build/ins && $(CMAKE_COMMAND) -P CMakeFiles/ins.dir/cmake_clean.cmake
.PHONY : ins/CMakeFiles/ins.dir/clean

ins/CMakeFiles/ins.dir/depend:
	cd /home/zmm/pl0compiler/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/zmm/pl0compiler /home/zmm/pl0compiler/ins /home/zmm/pl0compiler/build /home/zmm/pl0compiler/build/ins /home/zmm/pl0compiler/build/ins/CMakeFiles/ins.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : ins/CMakeFiles/ins.dir/depend

