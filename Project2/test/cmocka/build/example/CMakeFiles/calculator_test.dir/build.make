# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.7

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


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
CMAKE_SOURCE_DIR = /media/APES/Project2/test/cmocka

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /media/APES/Project2/test/cmocka/build

# Include any dependencies generated for this target.
include example/CMakeFiles/calculator_test.dir/depend.make

# Include the progress variables for this target.
include example/CMakeFiles/calculator_test.dir/progress.make

# Include the compile flags for this target's objects.
include example/CMakeFiles/calculator_test.dir/flags.make

example/CMakeFiles/calculator_test.dir/calculator.c.o: example/CMakeFiles/calculator_test.dir/flags.make
example/CMakeFiles/calculator_test.dir/calculator.c.o: ../example/calculator.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/media/APES/Project2/test/cmocka/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object example/CMakeFiles/calculator_test.dir/calculator.c.o"
	cd /media/APES/Project2/test/cmocka/build/example && /usr/bin/cc  $(C_DEFINES) -DUNIT_TESTING=1 $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/calculator_test.dir/calculator.c.o   -c /media/APES/Project2/test/cmocka/example/calculator.c

example/CMakeFiles/calculator_test.dir/calculator.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/calculator_test.dir/calculator.c.i"
	cd /media/APES/Project2/test/cmocka/build/example && /usr/bin/cc  $(C_DEFINES) -DUNIT_TESTING=1 $(C_INCLUDES) $(C_FLAGS) -E /media/APES/Project2/test/cmocka/example/calculator.c > CMakeFiles/calculator_test.dir/calculator.c.i

example/CMakeFiles/calculator_test.dir/calculator.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/calculator_test.dir/calculator.c.s"
	cd /media/APES/Project2/test/cmocka/build/example && /usr/bin/cc  $(C_DEFINES) -DUNIT_TESTING=1 $(C_INCLUDES) $(C_FLAGS) -S /media/APES/Project2/test/cmocka/example/calculator.c -o CMakeFiles/calculator_test.dir/calculator.c.s

example/CMakeFiles/calculator_test.dir/calculator.c.o.requires:

.PHONY : example/CMakeFiles/calculator_test.dir/calculator.c.o.requires

example/CMakeFiles/calculator_test.dir/calculator.c.o.provides: example/CMakeFiles/calculator_test.dir/calculator.c.o.requires
	$(MAKE) -f example/CMakeFiles/calculator_test.dir/build.make example/CMakeFiles/calculator_test.dir/calculator.c.o.provides.build
.PHONY : example/CMakeFiles/calculator_test.dir/calculator.c.o.provides

example/CMakeFiles/calculator_test.dir/calculator.c.o.provides.build: example/CMakeFiles/calculator_test.dir/calculator.c.o


example/CMakeFiles/calculator_test.dir/calculator_test.c.o: example/CMakeFiles/calculator_test.dir/flags.make
example/CMakeFiles/calculator_test.dir/calculator_test.c.o: ../example/calculator_test.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/media/APES/Project2/test/cmocka/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object example/CMakeFiles/calculator_test.dir/calculator_test.c.o"
	cd /media/APES/Project2/test/cmocka/build/example && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/calculator_test.dir/calculator_test.c.o   -c /media/APES/Project2/test/cmocka/example/calculator_test.c

example/CMakeFiles/calculator_test.dir/calculator_test.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/calculator_test.dir/calculator_test.c.i"
	cd /media/APES/Project2/test/cmocka/build/example && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /media/APES/Project2/test/cmocka/example/calculator_test.c > CMakeFiles/calculator_test.dir/calculator_test.c.i

example/CMakeFiles/calculator_test.dir/calculator_test.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/calculator_test.dir/calculator_test.c.s"
	cd /media/APES/Project2/test/cmocka/build/example && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /media/APES/Project2/test/cmocka/example/calculator_test.c -o CMakeFiles/calculator_test.dir/calculator_test.c.s

example/CMakeFiles/calculator_test.dir/calculator_test.c.o.requires:

.PHONY : example/CMakeFiles/calculator_test.dir/calculator_test.c.o.requires

example/CMakeFiles/calculator_test.dir/calculator_test.c.o.provides: example/CMakeFiles/calculator_test.dir/calculator_test.c.o.requires
	$(MAKE) -f example/CMakeFiles/calculator_test.dir/build.make example/CMakeFiles/calculator_test.dir/calculator_test.c.o.provides.build
.PHONY : example/CMakeFiles/calculator_test.dir/calculator_test.c.o.provides

example/CMakeFiles/calculator_test.dir/calculator_test.c.o.provides.build: example/CMakeFiles/calculator_test.dir/calculator_test.c.o


# Object files for target calculator_test
calculator_test_OBJECTS = \
"CMakeFiles/calculator_test.dir/calculator.c.o" \
"CMakeFiles/calculator_test.dir/calculator_test.c.o"

# External object files for target calculator_test
calculator_test_EXTERNAL_OBJECTS =

example/calculator_test: example/CMakeFiles/calculator_test.dir/calculator.c.o
example/calculator_test: example/CMakeFiles/calculator_test.dir/calculator_test.c.o
example/calculator_test: example/CMakeFiles/calculator_test.dir/build.make
example/calculator_test: src/libcmocka.so.0.4.1
example/calculator_test: example/CMakeFiles/calculator_test.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/media/APES/Project2/test/cmocka/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking C executable calculator_test"
	cd /media/APES/Project2/test/cmocka/build/example && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/calculator_test.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
example/CMakeFiles/calculator_test.dir/build: example/calculator_test

.PHONY : example/CMakeFiles/calculator_test.dir/build

example/CMakeFiles/calculator_test.dir/requires: example/CMakeFiles/calculator_test.dir/calculator.c.o.requires
example/CMakeFiles/calculator_test.dir/requires: example/CMakeFiles/calculator_test.dir/calculator_test.c.o.requires

.PHONY : example/CMakeFiles/calculator_test.dir/requires

example/CMakeFiles/calculator_test.dir/clean:
	cd /media/APES/Project2/test/cmocka/build/example && $(CMAKE_COMMAND) -P CMakeFiles/calculator_test.dir/cmake_clean.cmake
.PHONY : example/CMakeFiles/calculator_test.dir/clean

example/CMakeFiles/calculator_test.dir/depend:
	cd /media/APES/Project2/test/cmocka/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /media/APES/Project2/test/cmocka /media/APES/Project2/test/cmocka/example /media/APES/Project2/test/cmocka/build /media/APES/Project2/test/cmocka/build/example /media/APES/Project2/test/cmocka/build/example/CMakeFiles/calculator_test.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : example/CMakeFiles/calculator_test.dir/depend

