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
include lex/CMakeFiles/lex.dir/depend.make

# Include the progress variables for this target.
include lex/CMakeFiles/lex.dir/progress.make

# Include the compile flags for this target's objects.
include lex/CMakeFiles/lex.dir/flags.make

lex/CMakeFiles/lex.dir/pl0_lex.c.o: lex/CMakeFiles/lex.dir/flags.make
lex/CMakeFiles/lex.dir/pl0_lex.c.o: ../lex/pl0_lex.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/zmm/pl0compiler/build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object lex/CMakeFiles/lex.dir/pl0_lex.c.o"
	cd /home/zmm/pl0compiler/build/lex && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/lex.dir/pl0_lex.c.o   -c /home/zmm/pl0compiler/lex/pl0_lex.c

lex/CMakeFiles/lex.dir/pl0_lex.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/lex.dir/pl0_lex.c.i"
	cd /home/zmm/pl0compiler/build/lex && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /home/zmm/pl0compiler/lex/pl0_lex.c > CMakeFiles/lex.dir/pl0_lex.c.i

lex/CMakeFiles/lex.dir/pl0_lex.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/lex.dir/pl0_lex.c.s"
	cd /home/zmm/pl0compiler/build/lex && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /home/zmm/pl0compiler/lex/pl0_lex.c -o CMakeFiles/lex.dir/pl0_lex.c.s

lex/CMakeFiles/lex.dir/pl0_lex.c.o.requires:
.PHONY : lex/CMakeFiles/lex.dir/pl0_lex.c.o.requires

lex/CMakeFiles/lex.dir/pl0_lex.c.o.provides: lex/CMakeFiles/lex.dir/pl0_lex.c.o.requires
	$(MAKE) -f lex/CMakeFiles/lex.dir/build.make lex/CMakeFiles/lex.dir/pl0_lex.c.o.provides.build
.PHONY : lex/CMakeFiles/lex.dir/pl0_lex.c.o.provides

lex/CMakeFiles/lex.dir/pl0_lex.c.o.provides.build: lex/CMakeFiles/lex.dir/pl0_lex.c.o

# Object files for target lex
lex_OBJECTS = \
"CMakeFiles/lex.dir/pl0_lex.c.o"

# External object files for target lex
lex_EXTERNAL_OBJECTS =

lex/liblex.a: lex/CMakeFiles/lex.dir/pl0_lex.c.o
lex/liblex.a: lex/CMakeFiles/lex.dir/build.make
lex/liblex.a: lex/CMakeFiles/lex.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking C static library liblex.a"
	cd /home/zmm/pl0compiler/build/lex && $(CMAKE_COMMAND) -P CMakeFiles/lex.dir/cmake_clean_target.cmake
	cd /home/zmm/pl0compiler/build/lex && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/lex.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
lex/CMakeFiles/lex.dir/build: lex/liblex.a
.PHONY : lex/CMakeFiles/lex.dir/build

lex/CMakeFiles/lex.dir/requires: lex/CMakeFiles/lex.dir/pl0_lex.c.o.requires
.PHONY : lex/CMakeFiles/lex.dir/requires

lex/CMakeFiles/lex.dir/clean:
	cd /home/zmm/pl0compiler/build/lex && $(CMAKE_COMMAND) -P CMakeFiles/lex.dir/cmake_clean.cmake
.PHONY : lex/CMakeFiles/lex.dir/clean

lex/CMakeFiles/lex.dir/depend:
	cd /home/zmm/pl0compiler/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/zmm/pl0compiler /home/zmm/pl0compiler/lex /home/zmm/pl0compiler/build /home/zmm/pl0compiler/build/lex /home/zmm/pl0compiler/build/lex/CMakeFiles/lex.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : lex/CMakeFiles/lex.dir/depend

