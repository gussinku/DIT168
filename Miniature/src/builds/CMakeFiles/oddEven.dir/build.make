# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.5

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
CMAKE_SOURCE_DIR = /home/kundananji/Documents/DIT168/Miniature/src

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/kundananji/Documents/DIT168/Miniature/src/builds

# Include any dependencies generated for this target.
include CMakeFiles/oddEven.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/oddEven.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/oddEven.dir/flags.make

CMakeFiles/oddEven.dir/oddEven.cpp.o: CMakeFiles/oddEven.dir/flags.make
CMakeFiles/oddEven.dir/oddEven.cpp.o: ../oddEven.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/kundananji/Documents/DIT168/Miniature/src/builds/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/oddEven.dir/oddEven.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/oddEven.dir/oddEven.cpp.o -c /home/kundananji/Documents/DIT168/Miniature/src/oddEven.cpp

CMakeFiles/oddEven.dir/oddEven.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/oddEven.dir/oddEven.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/kundananji/Documents/DIT168/Miniature/src/oddEven.cpp > CMakeFiles/oddEven.dir/oddEven.cpp.i

CMakeFiles/oddEven.dir/oddEven.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/oddEven.dir/oddEven.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/kundananji/Documents/DIT168/Miniature/src/oddEven.cpp -o CMakeFiles/oddEven.dir/oddEven.cpp.s

CMakeFiles/oddEven.dir/oddEven.cpp.o.requires:

.PHONY : CMakeFiles/oddEven.dir/oddEven.cpp.o.requires

CMakeFiles/oddEven.dir/oddEven.cpp.o.provides: CMakeFiles/oddEven.dir/oddEven.cpp.o.requires
	$(MAKE) -f CMakeFiles/oddEven.dir/build.make CMakeFiles/oddEven.dir/oddEven.cpp.o.provides.build
.PHONY : CMakeFiles/oddEven.dir/oddEven.cpp.o.provides

CMakeFiles/oddEven.dir/oddEven.cpp.o.provides.build: CMakeFiles/oddEven.dir/oddEven.cpp.o


CMakeFiles/oddEven.dir/oddEvenChecker.cpp.o: CMakeFiles/oddEven.dir/flags.make
CMakeFiles/oddEven.dir/oddEvenChecker.cpp.o: ../oddEvenChecker.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/kundananji/Documents/DIT168/Miniature/src/builds/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/oddEven.dir/oddEvenChecker.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/oddEven.dir/oddEvenChecker.cpp.o -c /home/kundananji/Documents/DIT168/Miniature/src/oddEvenChecker.cpp

CMakeFiles/oddEven.dir/oddEvenChecker.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/oddEven.dir/oddEvenChecker.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/kundananji/Documents/DIT168/Miniature/src/oddEvenChecker.cpp > CMakeFiles/oddEven.dir/oddEvenChecker.cpp.i

CMakeFiles/oddEven.dir/oddEvenChecker.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/oddEven.dir/oddEvenChecker.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/kundananji/Documents/DIT168/Miniature/src/oddEvenChecker.cpp -o CMakeFiles/oddEven.dir/oddEvenChecker.cpp.s

CMakeFiles/oddEven.dir/oddEvenChecker.cpp.o.requires:

.PHONY : CMakeFiles/oddEven.dir/oddEvenChecker.cpp.o.requires

CMakeFiles/oddEven.dir/oddEvenChecker.cpp.o.provides: CMakeFiles/oddEven.dir/oddEvenChecker.cpp.o.requires
	$(MAKE) -f CMakeFiles/oddEven.dir/build.make CMakeFiles/oddEven.dir/oddEvenChecker.cpp.o.provides.build
.PHONY : CMakeFiles/oddEven.dir/oddEvenChecker.cpp.o.provides

CMakeFiles/oddEven.dir/oddEvenChecker.cpp.o.provides.build: CMakeFiles/oddEven.dir/oddEvenChecker.cpp.o


# Object files for target oddEven
oddEven_OBJECTS = \
"CMakeFiles/oddEven.dir/oddEven.cpp.o" \
"CMakeFiles/oddEven.dir/oddEvenChecker.cpp.o"

# External object files for target oddEven
oddEven_EXTERNAL_OBJECTS =

oddEven: CMakeFiles/oddEven.dir/oddEven.cpp.o
oddEven: CMakeFiles/oddEven.dir/oddEvenChecker.cpp.o
oddEven: CMakeFiles/oddEven.dir/build.make
oddEven: /usr/lib/libcluon.so
oddEven: CMakeFiles/oddEven.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/kundananji/Documents/DIT168/Miniature/src/builds/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable oddEven"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/oddEven.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/oddEven.dir/build: oddEven

.PHONY : CMakeFiles/oddEven.dir/build

CMakeFiles/oddEven.dir/requires: CMakeFiles/oddEven.dir/oddEven.cpp.o.requires
CMakeFiles/oddEven.dir/requires: CMakeFiles/oddEven.dir/oddEvenChecker.cpp.o.requires

.PHONY : CMakeFiles/oddEven.dir/requires

CMakeFiles/oddEven.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/oddEven.dir/cmake_clean.cmake
.PHONY : CMakeFiles/oddEven.dir/clean

CMakeFiles/oddEven.dir/depend:
	cd /home/kundananji/Documents/DIT168/Miniature/src/builds && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/kundananji/Documents/DIT168/Miniature/src /home/kundananji/Documents/DIT168/Miniature/src /home/kundananji/Documents/DIT168/Miniature/src/builds /home/kundananji/Documents/DIT168/Miniature/src/builds /home/kundananji/Documents/DIT168/Miniature/src/builds/CMakeFiles/oddEven.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/oddEven.dir/depend
