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
include src/CMakeFiles/cmocka_shared.dir/depend.make

# Include the progress variables for this target.
include src/CMakeFiles/cmocka_shared.dir/progress.make

# Include the compile flags for this target's objects.
include src/CMakeFiles/cmocka_shared.dir/flags.make

src/CMakeFiles/cmocka_shared.dir/cmocka.c.o: src/CMakeFiles/cmocka_shared.dir/flags.make
src/CMakeFiles/cmocka_shared.dir/cmocka.c.o: ../src/cmocka.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/media/APES/Project2/test/cmocka/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object src/CMakeFiles/cmocka_shared.dir/cmocka.c.o"
	cd /media/APES/Project2/test/cmocka/build/src && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/cmocka_shared.dir/cmocka.c.o   -c /media/APES/Project2/test/cmocka/src/cmocka.c

src/CMakeFiles/cmocka_shared.dir/cmocka.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/cmocka_shared.dir/cmocka.c.i"
	cd /media/APES/Project2/test/cmocka/build/src && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /media/APES/Project2/test/cmocka/src/cmocka.c > CMakeFiles/cmocka_shared.dir/cmocka.c.i

src/CMakeFiles/cmocka_shared.dir/cmocka.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/cmocka_shared.dir/cmocka.c.s"
	cd /media/APES/Project2/test/cmocka/build/src && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /media/APES/Project2/test/cmocka/src/cmocka.c -o CMakeFiles/cmocka_shared.dir/cmocka.c.s

src/CMakeFiles/cmocka_shared.dir/cmocka.c.o.requires:

.PHONY : src/CMakeFiles/cmocka_shared.dir/cmocka.c.o.requires

src/CMakeFiles/cmocka_shared.dir/cmocka.c.o.provides: src/CMakeFiles/cmocka_shared.dir/cmocka.c.o.requires
	$(MAKE) -f src/CMakeFiles/cmocka_shared.dir/build.make src/CMakeFiles/cmocka_shared.dir/cmocka.c.o.provides.build
.PHONY : src/CMakeFiles/cmocka_shared.dir/cmocka.c.o.provides

src/CMakeFiles/cmocka_shared.dir/cmocka.c.o.provides.build: src/CMakeFiles/cmocka_shared.dir/cmocka.c.o


# Object files for target cmocka_shared
cmocka_shared_OBJECTS = \
"CMakeFiles/cmocka_shared.dir/cmocka.c.o"

# External object files for target cmocka_shared
cmocka_shared_EXTERNAL_OBJECTS =

src/libcmocka.so.0.4.1: src/CMakeFiles/cmocka_shared.dir/cmocka.c.o
src/libcmocka.so.0.4.1: src/CMakeFiles/cmocka_shared.dir/build.make
src/libcmocka.so.0.4.1: src/CMakeFiles/cmocka_shared.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/media/APES/Project2/test/cmocka/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C shared library libcmocka.so"
	cd /media/APES/Project2/test/cmocka/build/src && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/cmocka_shared.dir/link.txt --verbose=$(VERBOSE)
	cd /media/APES/Project2/test/cmocka/build/src && $(CMAKE_COMMAND) -E cmake_symlink_library libcmocka.so.0.4.1 libcmocka.so.0 libcmocka.so

src/libcmocka.so.0: src/libcmocka.so.0.4.1
	@$(CMAKE_COMMAND) -E touch_nocreate src/libcmocka.so.0

src/libcmocka.so: src/libcmocka.so.0.4.1
	@$(CMAKE_COMMAND) -E touch_nocreate src/libcmocka.so

# Rule to build all files generated by this target.
src/CMakeFiles/cmocka_shared.dir/build: src/libcmocka.so

.PHONY : src/CMakeFiles/cmocka_shared.dir/build

src/CMakeFiles/cmocka_shared.dir/requires: src/CMakeFiles/cmocka_shared.dir/cmocka.c.o.requires

.PHONY : src/CMakeFiles/cmocka_shared.dir/requires

src/CMakeFiles/cmocka_shared.dir/clean:
	cd /media/APES/Project2/test/cmocka/build/src && $(CMAKE_COMMAND) -P CMakeFiles/cmocka_shared.dir/cmake_clean.cmake
.PHONY : src/CMakeFiles/cmocka_shared.dir/clean

src/CMakeFiles/cmocka_shared.dir/depend:
	cd /media/APES/Project2/test/cmocka/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /media/APES/Project2/test/cmocka /media/APES/Project2/test/cmocka/src /media/APES/Project2/test/cmocka/build /media/APES/Project2/test/cmocka/build/src /media/APES/Project2/test/cmocka/build/src/CMakeFiles/cmocka_shared.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/CMakeFiles/cmocka_shared.dir/depend

