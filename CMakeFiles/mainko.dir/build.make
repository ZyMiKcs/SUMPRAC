# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/vlad/summer_practice/SUMPRAC

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/vlad/summer_practice/SUMPRAC

# Include any dependencies generated for this target.
include CMakeFiles/mainko.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/mainko.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/mainko.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/mainko.dir/flags.make

CMakeFiles/mainko.dir/main.cpp.o: CMakeFiles/mainko.dir/flags.make
CMakeFiles/mainko.dir/main.cpp.o: main.cpp
CMakeFiles/mainko.dir/main.cpp.o: CMakeFiles/mainko.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/vlad/summer_practice/SUMPRAC/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/mainko.dir/main.cpp.o"
	$(CMAKE_COMMAND) -E __run_co_compile --tidy="clang-tidy-11;--extra-arg-before=--driver-mode=g++" --source=/home/vlad/summer_practice/SUMPRAC/main.cpp -- /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/mainko.dir/main.cpp.o -MF CMakeFiles/mainko.dir/main.cpp.o.d -o CMakeFiles/mainko.dir/main.cpp.o -c /home/vlad/summer_practice/SUMPRAC/main.cpp

CMakeFiles/mainko.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/mainko.dir/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/vlad/summer_practice/SUMPRAC/main.cpp > CMakeFiles/mainko.dir/main.cpp.i

CMakeFiles/mainko.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/mainko.dir/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/vlad/summer_practice/SUMPRAC/main.cpp -o CMakeFiles/mainko.dir/main.cpp.s

# Object files for target mainko
mainko_OBJECTS = \
"CMakeFiles/mainko.dir/main.cpp.o"

# External object files for target mainko
mainko_EXTERNAL_OBJECTS =

mainko: CMakeFiles/mainko.dir/main.cpp.o
mainko: CMakeFiles/mainko.dir/build.make
mainko: CMakeFiles/mainko.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/vlad/summer_practice/SUMPRAC/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable mainko"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/mainko.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/mainko.dir/build: mainko
.PHONY : CMakeFiles/mainko.dir/build

CMakeFiles/mainko.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/mainko.dir/cmake_clean.cmake
.PHONY : CMakeFiles/mainko.dir/clean

CMakeFiles/mainko.dir/depend:
	cd /home/vlad/summer_practice/SUMPRAC && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/vlad/summer_practice/SUMPRAC /home/vlad/summer_practice/SUMPRAC /home/vlad/summer_practice/SUMPRAC /home/vlad/summer_practice/SUMPRAC /home/vlad/summer_practice/SUMPRAC/CMakeFiles/mainko.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/mainko.dir/depend

