# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.9

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
CMAKE_SOURCE_DIR = /home/kstarxin/code/PaperCraft

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/kstarxin/code/PaperCraft

# Include any dependencies generated for this target.
include CMakeFiles/STG_Game.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/STG_Game.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/STG_Game.dir/flags.make

CMakeFiles/STG_Game.dir/src/SDL2_header.cpp.o: CMakeFiles/STG_Game.dir/flags.make
CMakeFiles/STG_Game.dir/src/SDL2_header.cpp.o: src/SDL2_header.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/kstarxin/code/PaperCraft/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/STG_Game.dir/src/SDL2_header.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/STG_Game.dir/src/SDL2_header.cpp.o -c /home/kstarxin/code/PaperCraft/src/SDL2_header.cpp

CMakeFiles/STG_Game.dir/src/SDL2_header.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/STG_Game.dir/src/SDL2_header.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/kstarxin/code/PaperCraft/src/SDL2_header.cpp > CMakeFiles/STG_Game.dir/src/SDL2_header.cpp.i

CMakeFiles/STG_Game.dir/src/SDL2_header.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/STG_Game.dir/src/SDL2_header.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/kstarxin/code/PaperCraft/src/SDL2_header.cpp -o CMakeFiles/STG_Game.dir/src/SDL2_header.cpp.s

CMakeFiles/STG_Game.dir/src/SDL2_header.cpp.o.requires:

.PHONY : CMakeFiles/STG_Game.dir/src/SDL2_header.cpp.o.requires

CMakeFiles/STG_Game.dir/src/SDL2_header.cpp.o.provides: CMakeFiles/STG_Game.dir/src/SDL2_header.cpp.o.requires
	$(MAKE) -f CMakeFiles/STG_Game.dir/build.make CMakeFiles/STG_Game.dir/src/SDL2_header.cpp.o.provides.build
.PHONY : CMakeFiles/STG_Game.dir/src/SDL2_header.cpp.o.provides

CMakeFiles/STG_Game.dir/src/SDL2_header.cpp.o.provides.build: CMakeFiles/STG_Game.dir/src/SDL2_header.cpp.o


CMakeFiles/STG_Game.dir/src/main.cpp.o: CMakeFiles/STG_Game.dir/flags.make
CMakeFiles/STG_Game.dir/src/main.cpp.o: src/main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/kstarxin/code/PaperCraft/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/STG_Game.dir/src/main.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/STG_Game.dir/src/main.cpp.o -c /home/kstarxin/code/PaperCraft/src/main.cpp

CMakeFiles/STG_Game.dir/src/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/STG_Game.dir/src/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/kstarxin/code/PaperCraft/src/main.cpp > CMakeFiles/STG_Game.dir/src/main.cpp.i

CMakeFiles/STG_Game.dir/src/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/STG_Game.dir/src/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/kstarxin/code/PaperCraft/src/main.cpp -o CMakeFiles/STG_Game.dir/src/main.cpp.s

CMakeFiles/STG_Game.dir/src/main.cpp.o.requires:

.PHONY : CMakeFiles/STG_Game.dir/src/main.cpp.o.requires

CMakeFiles/STG_Game.dir/src/main.cpp.o.provides: CMakeFiles/STG_Game.dir/src/main.cpp.o.requires
	$(MAKE) -f CMakeFiles/STG_Game.dir/build.make CMakeFiles/STG_Game.dir/src/main.cpp.o.provides.build
.PHONY : CMakeFiles/STG_Game.dir/src/main.cpp.o.provides

CMakeFiles/STG_Game.dir/src/main.cpp.o.provides.build: CMakeFiles/STG_Game.dir/src/main.cpp.o


CMakeFiles/STG_Game.dir/src/PaperCraft.cpp.o: CMakeFiles/STG_Game.dir/flags.make
CMakeFiles/STG_Game.dir/src/PaperCraft.cpp.o: src/PaperCraft.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/kstarxin/code/PaperCraft/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/STG_Game.dir/src/PaperCraft.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/STG_Game.dir/src/PaperCraft.cpp.o -c /home/kstarxin/code/PaperCraft/src/PaperCraft.cpp

CMakeFiles/STG_Game.dir/src/PaperCraft.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/STG_Game.dir/src/PaperCraft.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/kstarxin/code/PaperCraft/src/PaperCraft.cpp > CMakeFiles/STG_Game.dir/src/PaperCraft.cpp.i

CMakeFiles/STG_Game.dir/src/PaperCraft.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/STG_Game.dir/src/PaperCraft.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/kstarxin/code/PaperCraft/src/PaperCraft.cpp -o CMakeFiles/STG_Game.dir/src/PaperCraft.cpp.s

CMakeFiles/STG_Game.dir/src/PaperCraft.cpp.o.requires:

.PHONY : CMakeFiles/STG_Game.dir/src/PaperCraft.cpp.o.requires

CMakeFiles/STG_Game.dir/src/PaperCraft.cpp.o.provides: CMakeFiles/STG_Game.dir/src/PaperCraft.cpp.o.requires
	$(MAKE) -f CMakeFiles/STG_Game.dir/build.make CMakeFiles/STG_Game.dir/src/PaperCraft.cpp.o.provides.build
.PHONY : CMakeFiles/STG_Game.dir/src/PaperCraft.cpp.o.provides

CMakeFiles/STG_Game.dir/src/PaperCraft.cpp.o.provides.build: CMakeFiles/STG_Game.dir/src/PaperCraft.cpp.o


# Object files for target STG_Game
STG_Game_OBJECTS = \
"CMakeFiles/STG_Game.dir/src/SDL2_header.cpp.o" \
"CMakeFiles/STG_Game.dir/src/main.cpp.o" \
"CMakeFiles/STG_Game.dir/src/PaperCraft.cpp.o"

# External object files for target STG_Game
STG_Game_EXTERNAL_OBJECTS =

bin/STG_Game: CMakeFiles/STG_Game.dir/src/SDL2_header.cpp.o
bin/STG_Game: CMakeFiles/STG_Game.dir/src/main.cpp.o
bin/STG_Game: CMakeFiles/STG_Game.dir/src/PaperCraft.cpp.o
bin/STG_Game: CMakeFiles/STG_Game.dir/build.make
bin/STG_Game: /usr/lib/libSDL2main.a
bin/STG_Game: /usr/lib/libSDL2.so
bin/STG_Game: /usr/lib/libSDL2_image.so
bin/STG_Game: /usr/lib/libSDL2_ttf.so
bin/STG_Game: CMakeFiles/STG_Game.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/kstarxin/code/PaperCraft/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX executable bin/STG_Game"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/STG_Game.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/STG_Game.dir/build: bin/STG_Game

.PHONY : CMakeFiles/STG_Game.dir/build

CMakeFiles/STG_Game.dir/requires: CMakeFiles/STG_Game.dir/src/SDL2_header.cpp.o.requires
CMakeFiles/STG_Game.dir/requires: CMakeFiles/STG_Game.dir/src/main.cpp.o.requires
CMakeFiles/STG_Game.dir/requires: CMakeFiles/STG_Game.dir/src/PaperCraft.cpp.o.requires

.PHONY : CMakeFiles/STG_Game.dir/requires

CMakeFiles/STG_Game.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/STG_Game.dir/cmake_clean.cmake
.PHONY : CMakeFiles/STG_Game.dir/clean

CMakeFiles/STG_Game.dir/depend:
	cd /home/kstarxin/code/PaperCraft && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/kstarxin/code/PaperCraft /home/kstarxin/code/PaperCraft /home/kstarxin/code/PaperCraft /home/kstarxin/code/PaperCraft /home/kstarxin/code/PaperCraft/CMakeFiles/STG_Game.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/STG_Game.dir/depend

